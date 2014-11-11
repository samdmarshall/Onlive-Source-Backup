#include "PreComp.h"
#include "gfx_linux_common.h"
#include "gfx_linux_kernel.h"

/*******************************************************************************
**
**	gcoOS_GetBaseAddress
**
**	Get the base address for the physical memory.
**
**	INPUT:
**
**		gcoOS Os
**			Pointer to the gcoOS object.
**
**	OUTPUT:
**
**		gctUINT32_PTR BaseAddress
**			Pointer to a variable that will receive the base address.
*/
gceSTATUS
gcoOS_GetBaseAddress(
	IN 	gcoGALDEVICE Device,
	OUT gctUINT32_PTR BaseAddress
	)
{
	gcmVERIFY_ARGUMENT(BaseAddress != gcvNULL);

	/* Return base address. */
	*BaseAddress = Device->baseAddress;

	/* Success. */
	return gcvSTATUS_OK;
}

/*******************************************************************************
**
**	gcoOS_ReadRegister
**
**	Read data from a register.
**
**	INPUT:
**
**		gcoOS Os
**			Pointer to an gcoOS object.
**
**		gctUINT32 Address
**			Address of register.
**
**	OUTPUT:
**
**		gctUINT32 * Data
**			Pointer to a variable that receives the data read from the register.
*/
gceSTATUS gcoOS_ReadRegister(
	IN 	gcoGALDEVICE Device,
	IN gctUINT32 Address,
	OUT gctUINT32 * Data
	)
{
    /* Verify the arguments. */
    gcmVERIFY_ARGUMENT(Data != NULL);

    *Data = readl((gctUINT8 *)Device->registerBase + Address);
    gcmTRACE_ZONE("[gal]read 0x%x = 0x%x\n", (gctUINT8 *)Device->registerBase + Address, *Data);

    /* Success. */
    return gcvSTATUS_OK;
}

/*******************************************************************************
**
**	gcoOS_WriteRegister
**
**	Write data to a register.
**
**	INPUT:
**
**		gcoOS Os
**			Pointer to an gcoOS object.
**
**		gctUINT32 Address
**			Address of register.
**
**		gctUINT32 Data
**			Data for register.
**
**	OUTPUT:
**
**		Nothing.
*/
gceSTATUS gcoOS_WriteRegister(
	IN 	gcoGALDEVICE Device,
	IN gctUINT32 Address,
	IN gctUINT32 Data
	)
{
    gcmTRACE_ZONE("[gal]write 0x%x = 0x%x\n", (gctUINT8 *)Device->registerBase + Address, Data);

    writel(Data, (gctUINT8 *)Device->registerBase + Address);

    /* Success. */
    return gcvSTATUS_OK;
}

gceSTATUS gcoInterrupt_Get(
		IN gcoGALDEVICE Device,
		OUT gctUINT32 *Result)
{
    gctUINT32 storedData = 0;
#ifdef USE_NEW_INTERRUPT
	down_interruptible(&Device->sema);
#endif

	gcoOS_AtomicExchange(&Device->data, 0, &storedData);
	
	*Result = storedData;
    gcmTRACE_ZONE("[gal]IntGet 0x%x\n", storedData);	
    return gcvSTATUS_OK;
}

/*******************************************************************************
**
**	gcoKERNEL_Dispatch
**
**	Dispatch a command received from the user HAL layer.
**
**	INPUT:
**
**		gcoKERNEL Kernel
**			Pointer to an gcoKERNEL object.
**
**		gctBOOL FromUser
**			whether the call is from the user space.
**
**		gcsHAL_INTERFACE * Interface
**			Pointer to a gcsHAL_INTERFACE structure that defines the command to
**			be dispatched.
**
**	OUTPUT:
**
**		gcsHAL_INTERFACE * Interface
**			Pointer to a gcsHAL_INTERFACE structure that receives any data to be
**			returned.
*/
gceSTATUS gcoKERNEL_Dispatch(
	IN gcoGALDEVICE Device,
	IN gctBOOL FromUser,
	IN OUT gcsHAL_INTERFACE * Interface
	)
{
	gceSTATUS status;
	gctUINT32 bitsPerPixel;
	gctSIZE_T bytes;
	gcsHAL_INTERFACE * kernelInterface;

	/* Verify the arguments. */
	gcmVERIFY_ARGUMENT(Interface != gcvNULL);

	kernelInterface = Interface;

	/* Dispatch on command. */
	switch (kernelInterface->command)
	{
	case gcvHAL_GET_BASE_ADDRESS:
		/* Get base address. */
		status = gcoOS_GetBaseAddress(
			Device,
			&kernelInterface->u.GetBaseAddress.baseAddress);
		break;

    case gcvHAL_GET_INTERRUPT:
        /* Commit a command and context buffer. */
        status = gcoInterrupt_Get(
	        Device,
	        &kernelInterface->u.GetInterrupt.result);
        break;

    case gcvHAL_READ_REGISTER:
		/* Read a register. */
        status = gcoOS_ReadRegister(
			Device,
			kernelInterface->u.ReadRegisterData.address,
			&kernelInterface->u.ReadRegisterData.data);
        break;

    case gcvHAL_WRITE_REGISTER:
		/* Write a register. */
        status = gcoOS_WriteRegister(
			Device,
			kernelInterface->u.WriteRegisterData.address,
			kernelInterface->u.WriteRegisterData.data);
		break;

	default:
		/* Invalid command. */
		status = gcvSTATUS_INVALID_ARGUMENT;
		break;
	}

	/* Save status. */
	kernelInterface->status = status;

	/* Return the status. */
	return status;
}


