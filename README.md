# botcraft-pikmin-project

Small project to learn C++ and how to use Botcraft.

For linux, with some modifications can probably work for windows too.

libbotcraft.so and libprotocolCraft.so need to be put in libraries/ (create if doesnt exist)

Reads library files from a "commands" folder in the working directory, look through src/commands to see how it should be structured. Multiple commands can exist per library, however they all need to be added at the same time.

Requires a modification to Botcraft's source that makes Botcraft::World::SetBlockImpl a virtual function (allowing for override).

Also needs a modification that adds Botcraft::LoginType and makes Client->Connect take LoginType instead of a bool for third argument.
