#ifndef GDT_H_
#define GDT_H_

//  Type field
//  Bits 3:0
/*  Indicates the segment or gate type and specifies the kinds of
    access that can be made to the segment and the direction of growth.
    The interpretation of this field depends on whether thedescriptor
    type flag specifies an application (code or data) descriptor or a
    system descriptor. Theencoding of the type field is different for
    code, data, and system descriptors */
    // "Segment Accessed" bit 0
#define DATA_READ_ONLY                    0x0  // ....000.
#define DATA_READ_WRITE                   0x2  // ....001.
#define DATA_READ_ONLY_EXP_DOWN           0x4  // ....010.
#define DATA_READ_WRITE_EXP_DOWN          0x6  // ....011.
#define CODE_EXEC_ONLY                    0x8  // ....100.
#define CODE_EXEC_READ                    0xA  // ....101.
#define CODE_EXEC_ONLY_CONF               0xC  // ....110.
#define CODE_EXEC_READ_CONF               0xE  // ....111.
#define SEGM_ACCESSED                     0x1

//  S (DESCRIPTOR TYPE) FLAG
//  Bit 4
/*  Specifies whether the segment descriptor is for a system segment
    (S flag is clear) or a code or data
    segment (S flag is set). */
#define SYSTEM                            0x00
#define CODE_DATA_STACK                   0x10

//  DPL (descriptor privilege level) field
//  Bits 6:5
/*  Specifies the privilege level of the segment. The privilege level
    can range from 0 to 3, with 0 being the most privileged level.
    The DPL is used to control access to the segment */
#define RING_0                            0x00
#define RING_1                            0x20
#define RING_2                            0x40
#define RING_3                            0x60

//  P (segment-present) flag
//  Bit 7
/*  Indicates whether the segment is present in memory (set) or not present
    (clear). If this flag is clear, the processor generates a segment-not-present
    exception (#NP) when a segment selector that  points to the segment
    descriptor is loaded into a segment register */
#define NOT_PRESENT                       0x00
#define PRESENT                           0x80

//  L (64-bit code segment) flag
//  Bit 5
/*  In IA-32e mode, this bit of the segment descriptor indicates whether
    a code segment contains native 64-bit code. A value of 1 indicates instructions
    in this code segment are executed in 64-bit mode. A value of 0 indicates
    the instructions in this code segment are executed in compatibility mode.
    If L-bit is set, then D-bit must be cleared.
    When not in IA-32e mode or for non-code segments, this bit
    is reserved and should always be set to 0. */
#define MODE_IA32                         0x00
#define MODE_IA32e                        0x20

// D/B (default operation size/default stack pointer size and/or upper bound) flag
// Bit 6
/*  Performs different functions depending on whether the segment descriptor
    is an executable code segment, an expand-down data segment, or a stack segment.
    (This flag should always be set to 1 for 32-bit code and data segments
    and to 0 for 16-bit code and data segments.) */
#define BIT_16                             0x00
#define BIT_32                             0x40

//  G (granularity) flag
//  Bit 7
/*  Determines the scaling of the segment limit field. When the granularity flag
    is clear, the segment limit is interpreted in byte units; when flag is set,
    the segment limit is interpreted in 4-KByte units. */
#define _BYTE_                              0x00
#define _4KB_                               0x80
// GDT segment descriptor
typedef struct gdt_descriptor{
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t  base_middle;
  uint8_t  access;
  uint8_t  granularity;
  uint8_t  base_high;
} __attribute__((packed)) gdt_descriptor_t;

// GDT pointer
typedef struct gdt_ptr
{
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void gdt_install(void);

#endif
