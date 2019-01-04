/**
 * The address of the Application Interrupt and Reset Control Register (AIRCR).
 * We need to set bits in this register to trigger the reset.
 */
#define AIRCR_ADDR 0xE000ED0C

/**
 * The register key. We need to include this in our write operation or else the
 * cortex will ignore it.
 */
#define VECT_KEY 0x05FA

/**
 * This bit is "reserved for debug use", but the reset works when you set it.
 * Maybe setting it isn't necessary, but I haven't tested that.
 */
#define VECTRESET (1 << 0)

/**
 * The reset request bit. Setting it requests a reset.
 */
#define SYSRESETREQ (1 << 2)



//#include <stdint.h> // we need uint32_t

/**
 * reset - Causes the cortex to reset.
 * The cortex seems to handle this by restarting the operator control code,
 * when used in operator control mode. This function has not been tested in
 * autonomous, but probably doesn't need to be used there anyways.
 */
void reset(void)
{
	// get the AIRCR's contents
	uint32_t aircr = *((uint32_t *)AIRCR_ADDR);

	// preserve the other flags, set the register key then the reset bits
	aircr = (aircr & 0xFFFF) | (VECT_KEY << 16) | SYSRESETREQ | VECTRESET;

	// write the AIRCR (may not need to be volatile)
	*((volatile uint32_t *)AIRCR_ADDR) = aircr;

	// this instruction blocks execution until the write completes
	asm("DSB");

	// hang until the cortex resets
	while (1);
}
