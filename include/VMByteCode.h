#ifndef VMBYTECODE_H
#define VMBYTECODE_H

#include <stdint.h>

namespace vela
{
enum class vmByteCode
{
    stop,
    ist2op, // int stack to operator
    iop2st,
    ipushop, // int push operator
    ipopop,  // int pop operator
    iadd, // int add
    iprint, // int print
    iret // int return

};
}


#endif // VMBYTECODE_H
