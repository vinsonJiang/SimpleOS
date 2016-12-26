#ifndef INCLUDE_MM_PMM_H
#define INCLUDE_MM_PMM_H

#include <types.h>
#include <atomic.h>

// Ĭ��ջ�Ĵ�С(4096)
#define STACK_SIZE     (0x1000)

// �����ڴ�ҳ���С 
#define PMM_PAGE_SIZE  (0x1000)

// ҳ���� ���� 0x1000(4096) �����ַ
#define PMM_PAGE_MASK  (0xFFFFF000)

// �ں��������ڴ���ʼλ��
#define RAM_KERNEL_START (0x100000)



// ֧�ֵ���������ڴ�(512MB)
#define PMM_MAX_SIZE 0x20000000

// ���֧�ֵ�����ҳ�����
#define PAGE_MAX_SIZE (PMM_MAX_SIZE/PMM_PAGE_SIZE)




// �ں˴������ڴ��е���ʼ�ͽ���λ�ã������ӽű��ж���
extern uint8_t kern_start[];
extern uint8_t kern_end[];

// ������ҳ����֮����ں�ջ
extern uint8_t kern_stack[STACK_SIZE];

// �ں�ջ��ջ��
extern uint32_t kern_stack_top;

// BIOS int 0x15 AX = 0xE820 ����
#define E820MAX             (20)      // ���ı�����Ŀ
#define E820_ARM            (1)       // ���� RAM
#define E820_ARR            (2)       // ��������

typedef
struct e820map_t {
	uint32_t count;
	struct {
		uint32_t addr_low;
		uint32_t addr_high;
		uint32_t length_low;
		uint32_t length_high;
		uint32_t type;
	} __attribute__((packed)) map[E820MAX];
} e820map_t;

// �ڴ�ҳ����
typedef
enum mem_zone_t {
	ZONE_DMA = 0,
	ZONE_NORMAL = 1,
	ZONE_HIGHMEM = 2
} mem_zone_t;

#define ZONE_NORMAL_ADDR     (0x1000000)   // 16 MB
#define ZONE_HIGHMEM_ADDR    (0x38000000)  // 896 MB

// ����ҳ�ṹ
typedef
struct page_t {
	atomic_t ref;                // ����ҳ�����õĴ���
	uint32_t flag;               // ��ǰҳ״̬
	union {
		uint32_t ncount;     // ��ǰҳ��������ҳ������  First-Fit�㷨��Ҫ
		uint32_t order;      // ��ǰҳ�� order ֵ       buddy �㷨��Ҫ
	};
	struct list_head list;       // ������һ������ҳ
} page_t;

// page_t �� flag �����Ĳ�����
#define PG_RESERVED     (0)       // 1 << 0 ��ʾҳ��ǰ������
#define PG_NCOUNT       (1)       // 1 << 1 ��ʾ ncount �ֶ���Ч
#define PG_ORDER        (2)       // 1 << 2 ��ʾ order �ֶ���Ч

#define set_page_reserved_flag(page)       set_bit(PG_RESERVED, &((page)->flag))
#define clear_page_reserved_flag(page)     clear_bit(PG_RESERVED, &((page)->flag))
#define is_page_reserved(page)             test_bit(PG_RESERVED, &((page)->flag))

#define set_page_ncount_flag(page)          set_bit(PG_NCOUNT, &((page)->flag))
#define clear_page_ncount_flag(page)        clear_bit(PG_NCOUNT, &((page)->flag))
#define is_page_ncount(page)                test_bit(PG_NCOUNT, &((page)->flag))

#define set_page_order_flag(page)          set_bit(PG_ORDER, &((page)->flag))
#define clear_page_order_flag(page)        clear_bit(PG_ORDER, &((page)->flag))
#define is_page_order(page)                test_bit(PG_ORDER, &((page)->flag))

static inline int32_t page_ref(page_t *page)
{
	return atomic_read(&page->ref);
}

static inline void set_page_ref(page_t *page, int32_t val)
{
	atomic_set(&page->ref, val);
}

static inline void page_ref_inc(page_t *page)
{
	atomic_inc(&page->ref);
}

static inline void page_ref_dec(page_t *page)
{
	atomic_dec(&page->ref);
}

// �������ַ������õ�ַ����ҳ�Ĺ���ṹ��ָ��
page_t *addr_to_page(uint32_t addr);

// ��ҳ�Ĺ���ṹ��ָ������ҳ���������ַ
uint32_t page_to_addr(page_t *page);

// �ڴ������ϵͳ�������
struct pmm_manager {
	const char *name;                                // �����㷨������
	void(*page_init)(page_t *pages, uint32_t n);    // ��ʼ��
	uint32_t(*alloc_pages)(uint32_t n);             // ���������ڴ�ҳ(nΪ�ֽ���)
	void(*free_pages)(uint32_t addr, uint32_t n);   // �ͷ��ڴ�ҳ
	uint32_t(*free_pages_count)(void);              // ���ص�ǰ�����ڴ�ҳ
};

// �����ڴ�����ʼ��
void pmm_init(void);

// �ڴ�����㷨��ʼ��
void page_init(page_t *pages, uint32_t n);

// �����ڴ�ҳ
uint32_t alloc_pages(uint32_t n);

// �ͷ��ڴ�ҳ
void free_pages(uint32_t addr, uint32_t n);

#define alloc_page alloc_pages(1)
#define free_page(addr) free_pages(addr, 1)

// ��ǰ�����ڴ�ҳ
uint32_t free_pages_count(void);

#endif  // INCLUDE_MM_PMM_H
