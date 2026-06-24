---
name: safety-critical-cpp
description: Production C++ for embedded, RTOS, safety-relevant, MISRA C++-oriented code. Use when writing or reviewing C++ where deterministic behavior, analyzability, ownership, lifetime, and future safety review matter.
---

# Safety Critical C++

Use this skill for production embedded C++ and safety-relevant C++ review.

## Core Guidance

- Do not claim MISRA C++ compliance without static-analysis evidence, deviation records, review evidence, and project approval.
- Say "MISRA C++-oriented" or "prepared for MISRA C++ review" instead.
- Prefer simple, explicit, statically analyzable C++.
- Prefer RAII only when destructors are `noexcept`, bounded, and non-blocking.
- Avoid exceptions, RTTI, recursion, unbounded STL containers, unbounded queues, and dynamic allocation in real-time or safety paths unless explicitly approved.
- Prefer `std::array`, `constexpr`, `enum class`, explicit constructors, narrow interfaces, move-only ownership wrappers, and fixed-capacity project containers.
- Flag hidden allocation, destructor side effects, implicit conversions, unsafe lambda captures, use-after-move risk, static initialization order risk, and ownership ambiguity.
- Keep failure behavior explicit. Prefer named states and explicit validation over implicit defaults.
- Record assumptions that affect timing, memory, ownership, concurrency, diagnostics, or reviewability.

## References

Read the relevant reference before writing or reviewing safety-relevant C++:

- `references/misra-cpp-2023.md`
- `references/memory-management-rules.md`
- `references/concurrency-rules.md`
- `references/real-time-coding-rules.md`
- `references/code-review-checklist.md`
