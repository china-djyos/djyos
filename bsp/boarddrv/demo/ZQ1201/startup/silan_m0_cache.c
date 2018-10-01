#include "silan_types.h"
#include "silan_m0_cache_regs.h"
#include "silan_uart_regs.h"
#include "silan_printf.h"
#include "silan_pmu.h"
#include "silan_m0_cache.h"

static void cache_clear(void)
{
	spicache_st->cache_ctrl = CACHE_AUTOCLEAR;
	while (!(spicache_st->cache_ctrl & CACHE_AUTOCLEAR_END));
	spicache_st->cache_ctrl = CACHE_AUTOCLEAR_END;
}
static void cache_area_sel(u32 cache_area)
{
	spicache_st->main_ctrl &= ~(CACHE_AREA_1|CACHE_AREA_2|CACHE_AREA_3|CACHE_AREA_4);	
	spicache_st->main_ctrl |= cache_area;
}
static void cache_enable(void)
{ 
	spicache_st->main_ctrl |= CACHE_ENABLE;
}
static void cache_disable(void)
{
	spicache_st->main_ctrl &= ~CACHE_ENABLE;
}
static void cache_ram_en(void)
{
	spicache_st->main_ctrl |= CACHE_RAM_EN ;
}
static void cache_ram_disable(void)
{
	spicache_st->main_ctrl &=~CACHE_RAM_EN ;
}
static void cache_statistics_disable(void)
{
	spicache_st->cache_ctrl &= ~CACHE_STATISTICS_ENABLE;
}
static void cache_statistics_enable(void)
{
	spicache_st->cache_ctrl |= CACHE_STATISTICS_ENABLE;
}
static u32 cache_load_code(u32 addr, u32 len)
{
	spicache_st->opaddr = addr;
	spicache_st->opsize = len;
	spicache_st->cache_ctrl = CACHE_AUTOFILL;
	while (!(spicache_st->cache_ctrl & CACHE_AUTOFILL_END));
	spicache_st->cache_ctrl = CACHE_AUTOFILL_END;
	return 0;
}
static u32 cache_lock_code(u32 addr, u32 len)
{
	spicache_st->opaddr = addr;
	spicache_st->opsize = len;
	spicache_st->cache_ctrl = CACHE_AUTOFILL | CACHE_AUTOFILL_LOCK;
	while (!(spicache_st->cache_ctrl & CACHE_AUTOFILL_END));
	spicache_st->cache_ctrl = CACHE_AUTOFILL_END;
	return 0;
}
void cache_unlock_code(u32 addr, u32 len,u8 need_cache_unvalid)
{
	spicache_st->opaddr = addr;
	spicache_st->opsize = len;
	if(need_cache_unvalid)
		spicache_st->cache_ctrl = CACHE_UNLOCK|CACHE_UNVALID;
	else
		spicache_st->cache_ctrl = CACHE_UNLOCK;
	while (!(spicache_st->cache_ctrl & CACHE_UNLOCK_END));
	spicache_st->cache_ctrl = CACHE_UNLOCK_END;
}

void m0_cache_init(u8 operation,u8 if_lock,u32 lock_addr,u32 len)
{
		cache_clear();
		cache_area_sel(CACHE_AREA_1);
		if(operation){
				if(if_lock){
					if(len<=0xff)
						cache_lock_code(lock_addr,len);
					else if(len<=0x1fe){
						cache_lock_code(lock_addr,0xff);
						cache_lock_code(lock_addr+16*255,len-0xff);
					}else {
						cache_lock_code(lock_addr,0xff);
						cache_lock_code(lock_addr+16*255,0xff);
						cache_lock_code(lock_addr+16*255*2,len-0x1fe);
					}
				}
				cache_ram_en();
				cache_ram_disable();
				cache_enable();
		}else{	
				cache_disable();
		}

}


/*add 2018/9/27 */

void silan_m0_cache_init(uint32_t cache_area, uint8_t statistics)
{
	cache_clear();
	cache_area_sel(cache_area);
	
	if(statistics)
		cache_statistics_enable();
	else
		cache_statistics_disable();
}

void silan_m0_cache_lock(uint32_t lock_addr, uint32_t len)
{
	if(len<=0xff)
		cache_lock_code(lock_addr,len);
	else if(len<=0x1fe){
		cache_lock_code(lock_addr,0xff);
		cache_lock_code(lock_addr+16*255,len-0xff);
	}else {
		cache_lock_code(lock_addr,0xff);
		cache_lock_code(lock_addr+16*255,0xff);
		cache_lock_code(lock_addr+16*255*2,len-0x1fe);
	}
}

void silan_m0_cache_unlock(uint32_t lock_addr, uint32_t len, uint8_t unvalid)
{
	if(len<=0xff)
		cache_unlock_code(lock_addr,len, unvalid);
	else if(len<=0x1fe){
		cache_unlock_code(lock_addr,0xff, unvalid);
		cache_unlock_code(lock_addr+16*255,len-0xff, unvalid);
	}else {
		cache_unlock_code(lock_addr,0xff, unvalid);
		cache_unlock_code(lock_addr+16*255,0xff, unvalid);
		cache_unlock_code(lock_addr+16*255*2,len-0x1fe, unvalid);
	}
}

void silan_m0_cache_enable(void)
{
	SL_DBGLOG("m0 cache enable");
	cache_enable();
}

void silan_m0_cache_disable(void)
{
	cache_disable();
}

void silan_m0_cache_clear(void)
{
    cache_clear();
}





