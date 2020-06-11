#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <int.h>

uint32_t platform_is_in_interrupt_context( void )
{
    return Int_GetRunLevel();
}