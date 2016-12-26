#ifndef INCLUDE_MM_VMM_H
#define INCLUDE_MM_VMM_H

#include <arch.h>
#include <types.h>
#include <lib/list.h>

/* *
* Virtual memory map:
*
*
*     4G ------------------> +---------------------------------+
*                            |                                 |
*                            |         Empty Memory (*)        |
*                            |                                 |
*                            +---------------------------------+ 0xF8000000
*                            |                                 |
*                            |                                 |
*                            |    Remapped Physical Memory     | RW/-- KMEMSIZE
*                            |                                 |
*                            |                                 |
*                            +---------------------------------+
*                            |             Kernel              |
*     KERNBASE ------------> +---------------------------------+ 0xC0000000
*                            |        Invalid Memory (*)       | --/--
*     USERTOP -------------> +---------------------------------+ 0xB0000000
*                            |           User stack            |
*                            +---------------------------------+
*                            :                                 :
*                            |         ~~~~~~~~~~~~~~~~        |
*                            :                                 :
*                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*                            |       User Program & Heap       |
*     UTEXT ---------------> +---------------------------------+ 0x00800000
*                            |        Invalid Memory (*)       | --/--
*      USERBASE -----------> +---------------------------------+ 0x00200000
*                            |        Invalid Memory (*)       | --/--
*     0 -------------------> +---------------------------------+ 0x00000000
* (*) Note: The kernel ensures that "Invalid Memory" is *never* mapped.
*     "Empty Memory" is normally unmapped, but user programs may map pages
*     there if desired.
*
* */

// A linear address 'la' has a three-part structure as follows:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |     Index      |                     |
// +----------------+----------------+---------------------+
//   \PGD_INDEX(la)/ \ PTE_INDEX(la) /  \OFFSET_INDEX(la)/

// �����ҳ��С(4KB)
#define PAGE_SIZE 	  (0x1000)

// ҳ���룬���� 4KB ����
#define PAGE_MASK         (0xFFFFF000)

// �ں���ʼ�����ַ
#define KERNBASE          (0xC0000000)

// �ں˹����ڴ�Ĵ�С
#define KMEMSIZE          (0x38000000)

// �ں�ҳ���С
#define KVPAGE_SIZE       (0x400000)

// �ں˹���������ڴ�Ķ��˵�ַ
#define KERNTOP           (KERNBASE + KMEMSIZE)

// �ں˵�ƫ�Ƶ�ַ
#define PAGE_OFFSET 	  KERNBASE

// ÿ��ҳ�����ӳ����ڴ���
#define PAGE_MAP_SIZE 	  (0x400000)

// ӳ�� KMEM_SIZE ���ڴ�����Ҫ��ҳ��
#define PTE_COUNT         (KMEMSIZE/PAGE_MAP_SIZE)

// ��ȡһ����ַ��ҳĿ¼��
#define PGD_INDEX(x)      (((x) >> 22) & 0x3FF)

// ��ȡһ����ַ��ҳ����
#define PTE_INDEX(x)      (((x) >> 12) & 0x3FF)

// ��ȡһ����ַ��ҳ��ƫ��
#define OFFSET_INDEX(x)   ((x) & 0xFFF)

// P--λ0�Ǵ��ڱ�ʶ��Ϊ 1 ���ڴ�ҳ���ڴ���
#define PAGE_PRESENT 	(0x1)

// R/W--λ1�Ƕ�/д��ʶ��������� 1����ʾҳ����Ա�����д��ִ�С�
#define PAGE_WRITE 	(0x2)

// U/S--λ2���û�/�����û���ʶ��Ϊ 1 ���κ���Ȩ���ϵĳ��򶼿��Է��ʸ�ҳ�档
#define PAGE_USER 	(0x4)

// ҳ���Ա��
#define PGD_SIZE (PAGE_SIZE/sizeof(pte_t))

// ҳ���Ա��
#define PTE_SIZE (PAGE_SIZE/sizeof(uint32_t))

// �����ڴ���Ϣ
struct mm_struct {
	pgd_t *pgdir;
	int vma_count;
	struct list_head vma_list;
};

// ���������ڴ�����
struct vma_struct {
	struct mm_struct *mm;
	uint32_t vm_start;
	uint32_t vm_end;
	uint32_t vm_flags;
	struct list_head list;
};

#define le_to_vma(le) list_entry(le, struct vma_struct, list)

#define VM_READ       (1u << 0)
#define VM_WRITE      (1u << 1)
#define VM_EXEC       (1u << 2)

// �����ַת���ں������ַ
static inline void *pa_to_ka(void *pa)
{
	return (void *)((uint32_t)pa + KERNBASE);
}

// �ں������ַת�������ַ
static inline void *ka_to_pa(void *ka)
{
	return (void *)((uint32_t)ka - KERNBASE);
}

// �ں�ҳĿ¼����
extern pgd_t pgd_kern[];

// �����ڴ�����ʼ��
void vmm_init(void);

// ʹ�� flags ָ����ҳȨ�ޣ��������ַ pa ӳ�䵽�����ַ va
void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags);

// ȡ�������ַ va ������ӳ��
void unmap(pgd_t *pgd_now, uint32_t va);

// ��������ַ va ӳ�䵽�����ַ�򷵻� 1
// ͬʱ��� pa ���ǿ�ָ����������ַд�� pa ����
uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa);

// ҳ�����жϵĺ�������
void do_page_fault(pt_regs_t *regs);

#endif 	// INCLUDE_MM_VMM_H
