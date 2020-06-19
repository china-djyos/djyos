#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <int.h>

uint32_t platform_is_in_interrupt_context( void )
{
    return Int_GetRunLevel();
}

uint32_t rt_hw_interrupt_disable(void)
{
    return Int_HighAtomStart();
}

void rt_hw_interrupt_enable(uint32_t level)
{
    Int_HighAtomEnd(level);
}
