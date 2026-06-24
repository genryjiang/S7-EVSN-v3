# Creating and Using Types
Most standard type classes should already be created in the `std_types` directory, ready to use. To use one in your node, simply `#include "std_types.hpp"`, and then use any of the classes. For example `EVSNInt32 my_msg{100}`. These types are essential as each topic needs a type, and to publish and subscribe to a topic, you need to work with instances of these type classes. To see an example of this, please read `S7-EVSN/src/cat_shelter_example`.

If you need a topic to store an **array** of one of these primative types, talk to `Ryan Wong z5417983` and he will create more standard types which are arrays of primatives

If you need to make new custom types, like more complex structs, please read the following.
## Creating a Custom Type
1. Create a new directory in `evsn-runtime/dds_types/custom_types`.
    - The directory name should be in snake_case and have an `evsn_` prefix. For example: `evsn_bms_cell_voltage`
2. In your developer environment container you should have access to the `rtiddsgen` command. Try running `rtiddsgen` and you should see a help message pop up.
3. Create an .idl file descrbing your message type. For example:
```c
// evsn_bms_cell_voltage.idl
struct EVSNBmsCellVoltage {
    uint32 cell_id;
    float voltage;
};
```
- The syntax is very similar to C, but the types are listed [HERE](https://community.rti.com/static/documentation/connext-dds/current/doc/manuals/connext_dds_professional/users_manual/users_manual/Translations_for_IDL_Types.htm) (scroll down to *Table 20.7 Specifying Data Types in IDL for Modern C++*)
- Make sure to name your type descriptively and use CamelCase like any other C++ class.

4. Now with your evsn_bms_cell_voltage.idl file, making sure you're in the directory you created under custom_types, run:
```bash
rtiddsgen -language C++11 -create typefiles evsn_bms_cell_voltage.idl
```
- This should create 4 files in your directory.
5. Next go to `custom_types/custom_types.hpp` and add an include statement to `evsn_bms_cell_voltage/evsn_bms_cell_voltage.hpp` or whatever your type name is. NOT the plugin header.

6. You should be good to go! Now you can `#include "custom_types.hpp"` to have access to ALL custom types defined in that header file. Simply instantiate the class which name is the same as your struct name from the IDL. 
- To retrieve data from a message instance. Do msg.voltage() or whatever your struct's fields are.

Written by: Ryan Wong | z5417983



