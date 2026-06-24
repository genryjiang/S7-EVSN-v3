#include <ha/ham.h>

#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static void sleep_ms(long millis) {
    struct timespec delay;
    delay.tv_sec = millis / 1000;
    delay.tv_nsec = (millis % 1000) * 1000000L;
    nanosleep(&delay, NULL);
}

static int write_marker(const char* path, const char* message) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "open marker %s: %s\n", path, strerror(errno));
        return 1;
    }
    fprintf(file, "%s\n", message);
    fclose(file);
    return 0;
}

static void marker_path(char* out, size_t out_size, const char* prefix, const char* suffix) {
    snprintf(out, out_size, "%s.%s", prefix, suffix);
}

static void require_condition(ham_condition_t* condition, const char* label) {
    if (condition == NULL) {
        fprintf(stderr, "%s: %s\n", label, strerror(errno));
        exit(2);
    }
}

static void require_action(ham_action_t* action, const char* label) {
    if (action == NULL) {
        fprintf(stderr, "%s: %s\n", label, strerror(errno));
        exit(2);
    }
}

static void configure_ham_actions(ham_entity_t* entity, const char* prefix, const char* restart_command) {
    char high_marker[256];
    char high_command[512];
    marker_path(high_marker, sizeof(high_marker), prefix, "high");
    snprintf(high_command, sizeof(high_command), "/tmp/sr_ham_smoke --mark %s high", high_marker);

    ham_condition_t* death = ham_condition(
        entity,
        CONDDEATH,
        "death",
        HCONDNOWAIT | HREARMAFTERRESTART);
    require_condition(death, "ham_condition death");
    require_action(
        ham_action_log(
            death,
            "death_log",
            "SRNode HAM smoke death condition",
            1,
            1,
            HREARMAFTERRESTART),
        "ham_action_log death");
    require_action(
        ham_action_restart(
            death,
            "restart",
            restart_command,
            HREARMAFTERRESTART | HACTIONBREAKONFAIL),
        "ham_action_restart death");

    ham_condition_t* low = ham_condition(
        entity,
        CONDHBEATMISSEDLOW,
        "heartbeat_low",
        HCONDNOWAIT | HREARMAFTERRESTART);
    require_condition(low, "ham_condition heartbeat_low");
    require_action(
        ham_action_log(
            low,
            "heartbeat_low_log",
            "SRNode HAM smoke low missed heartbeat",
            1,
            1,
            HREARMAFTERRESTART),
        "ham_action_log heartbeat_low");

    ham_condition_t* high = ham_condition(
        entity,
        CONDHBEATMISSEDHIGH,
        "heartbeat_high",
        HCONDNOWAIT | HREARMAFTERRESTART);
    require_condition(high, "ham_condition heartbeat_high");
    require_action(
        ham_action_log(
            high,
            "heartbeat_high_log",
            "SRNode HAM smoke high missed heartbeat",
            1,
            1,
            HREARMAFTERRESTART),
        "ham_action_log heartbeat_high");
    require_action(
        ham_action_execute(
            high,
            "heartbeat_high_marker",
            high_command,
            HREARMAFTERRESTART | HACTIONBREAKONFAIL),
        "ham_action_execute heartbeat_high");
    require_action(
        ham_action_heartbeat_healthy(
            high,
            "heartbeat_high_healthy",
            HREARMAFTERRESTART),
        "ham_action_heartbeat_healthy heartbeat_high");
}

static int run_worker(const char* entity_name, const char* prefix, const char* role) {
    char attached_marker[256];
    char alive_marker[256];
    char restarting_marker[256];
    char restarted_marker[256];
    char detached_marker[256];
    char restart_command[512];

    marker_path(attached_marker, sizeof(attached_marker), prefix, "attached");
    marker_path(alive_marker, sizeof(alive_marker), prefix, "alive");
    marker_path(restarting_marker, sizeof(restarting_marker), prefix, "restarting");
    marker_path(restarted_marker, sizeof(restarted_marker), prefix, "restarted");
    marker_path(detached_marker, sizeof(detached_marker), prefix, "detached");
    snprintf(
        restart_command,
        sizeof(restart_command),
        "/tmp/sr_ham_smoke --worker %s %s restarted",
        entity_name,
        prefix);

    ham_entity_t* entity = ham_attach_self(entity_name, 1000000000ULL, 2, 3, 0);
    if (entity == NULL) {
        fprintf(stderr, "ham_attach_self %s: %s\n", entity_name, strerror(errno));
        return 2;
    }

    configure_ham_actions(entity, prefix, restart_command);
    write_marker(attached_marker, role);

    for (int i = 0; i < 3; ++i) {
        ham_heartbeat();
        sleep_ms(1000);
    }

    sleep_ms(4000);

    for (int i = 0; i < 5; ++i) {
        ham_heartbeat();
        sleep_ms(1000);
    }
    write_marker(alive_marker, role);

    if (strcmp(role, "initial") == 0) {
        write_marker(restarting_marker, "initial");
        kill(getpid(), SIGKILL);
        return 3;
    }

    write_marker(restarted_marker, role);
    if (ham_detach_self(entity, 0) == -1) {
        fprintf(stderr, "ham_detach_self: %s\n", strerror(errno));
        return 2;
    }
    write_marker(detached_marker, role);
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 4 && strcmp(argv[1], "--mark") == 0) {
        return write_marker(argv[2], argv[3]);
    }

    if (argc == 5 && strcmp(argv[1], "--worker") == 0) {
        return run_worker(argv[2], argv[3], argv[4]);
    }

    fprintf(
        stderr,
        "Usage: %s --worker <entity> <marker-prefix> <initial|restarted> | "
        "--mark <path> <message>\n",
        argv[0]);
    return 2;
}
