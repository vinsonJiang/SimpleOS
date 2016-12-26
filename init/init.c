#include <common.h>
#include <debug.h>
#include <init.h>
#include <mm/mm.h>

// ������ҳ����֮��� Multiboot ����ָ��
multiboot_t *glb_mboot_ptr;

// ������ҳ����֮����ں�ջ
uint8_t kern_stack[STACK_SIZE]  __attribute__((aligned(STACK_SIZE)));

// �ں�ջ��ջ��
uint32_t kern_stack_top = (uint32_t)kern_stack + STACK_SIZE;

// �ں�ʹ�õ���ʱҳ���ҳĿ¼
// �õ�ַ������ҳ����ĵ�ַ���ڴ� 0-640KB �϶��ǿ��е�
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pte_t *pte_low = (pte_t *)0x2000;
__attribute__((section(".init.data"))) pte_t *pte_hign = (pte_t *)0x3000;

// ӳ����ʱҳ��
__attribute__((section(".init.text"))) void mmap_tmp_page(void);

// ���÷�ҳ
__attribute__((section(".init.text"))) void enable_paging(void);

// �ں���ں���
__attribute__((section(".init.text"))) void kern_entry(void)
{
	// ӳ����ʱҳ��
	mmap_tmp_page();

	// ���÷�ҳ
	enable_paging();

	// �л���ʱ�ں�ջ����ҳ�����ջ
	__asm__ volatile ("mov %0, %%esp\n\t"
		"xor %%ebp, %%ebp" : : "r" (kern_stack_top));

	// ����ȫ�� multiboot_t ָ��ָ��
	glb_mboot_ptr = (multiboot_t *)((uint32_t)mboot_ptr_tmp + PAGE_OFFSET);

	// �����ں˳�ʼ������
	kern_init();

	// ֮ǰ�ĺ�����������ջ�л���Ͽ����޷��ٷ���֮ǰ�ĵ��õ�
}

// ӳ����ʱҳ��
__attribute__((section(".init.text"))) void mmap_tmp_page(void)
{
	pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;

	for (int i = 0; i < 4; ++i) {
		uint32_t pgd_idx = PGD_INDEX(PAGE_OFFSET + PAGE_MAP_SIZE * i);
		pgd_tmp[pgd_idx] = ((uint32_t)pte_hign + PAGE_SIZE * i) | PAGE_PRESENT | PAGE_WRITE;
	}

	// ӳ���ں������ַ 4MB �������ַ��ǰ 4MB
	// ��Ϊ .init.text �εĴ����������ַǰ 4MB ��(�϶����ᳬ�������Χ)��
	// ������ҳ�����˴���ӳ�䣬����ִ������������뿪 .init.text �κ�Ĵ���ִ�У�
	// ������Ҫӳ������ǰ 4MB ���ڴ�
	for (int i = 0; i < 1024; i++) {
		pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	// ӳ�� 0x00000000-0x01000000 �������ַ�������ַ 0xC0000000-0xC1000000
	for (int i = 0; i < 1024 * 4; i++) {
		pte_hign[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	// ������ʱҳ��
	__asm__ volatile ("mov %0, %%cr3" : : "r" (pgd_tmp));
}

// ���÷�ҳ
__attribute__((section(".init.text"))) void enable_paging(void)
{
	uint32_t cr0;

	__asm__ volatile ("mov %%cr0, %0" : "=r" (cr0));
	// ���λ PG λ�� 1����ҳ����
	cr0 |= (1u << 31);
	__asm__ volatile ("mov %0, %%cr0" : : "r" (cr0));
}