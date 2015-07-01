/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Drivers:NAND
 * Copyright (c) 2004-2013 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    OneNAND_Config.h
 * Purpose: OneNAND Flash Device connected to Memory Bus Configuration
 * Rev.:    V5.00
 *----------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>OneNAND Driver Configuration

//   <o>Connect to hardware via Driver_NAND# <0-255>
//   <i>Select driver control block for hardware interface
#define NAND_DRIVER             0

//   <e>OneNAND Device 0
//     <o1>Base Address
//     <q2>Ready/Busy Pin Interrupt
//   </e>
#define NAND_DEV0               1
#define NAND_DEV0_ADDR_BASE     0x80000000
#define NAND_DEV0_RB_PIN_IRQ    0

//   <e>OneNAND Device 1
//     <o1>Base Address
//     <q2>Ready/Busy Pin Interrupt
//   </e>
#define NAND_DEV1               0
#define NAND_DEV1_ADDR_BASE     0x81000000
#define NAND_DEV1_RB_PIN_IRQ    0

//   <e>OneNAND Device 2
//     <o1>Base Address
//     <q2>Ready/Busy Pin Interrupt
//   </e>
#define NAND_DEV2               0
#define NAND_DEV2_ADDR_BASE     0x82000000
#define NAND_DEV2_RB_PIN_IRQ    0

//   <e>OneNAND Device 3
//     <o1>Base Address
//     <q2>Ready/Busy Pin Interrupt
//   </e>
#define NAND_DEV3               0
#define NAND_DEV3_ADDR_BASE     0x83000000
#define NAND_DEV3_RB_PIN_IRQ    0

// </h>
