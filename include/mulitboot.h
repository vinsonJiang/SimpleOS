#ifndef INCLUDE_MULTIBOOT_H_
#define INCLUDE_MULTIBOOT_H_

#include <types.h>

/**
* ��������32λ�ں˽���㣬����״̬���£�
*   1. CS ָ�����ַΪ 0x00000000���޳�Ϊ4G �C 1�Ĵ������������
*   2. DS��SS��ES��FS �� GS ָ�����ַΪ0x00000000���޳�Ϊ4G �C 1�����ݶ���������
*   3. A20 ��ַ���Ѿ��򿪡�
*   4. ҳ���Ʊ���ֹ��
*   5. �жϱ���ֹ��
*   6. EAX = 0x2BADB002
*   7. ϵͳ��Ϣ��������Ϣ������Ե�ַ������ EBX�У��൱��һ��ָ�룩��
*      ���¼�Ϊ�����Ϣ��Ľṹ
*/

typedef
struct multiboot_t {
	uint32_t flags;                 // Multiboot �İ汾��Ϣ
									/**
									* �� BIOS ��֪�Ŀ����ڴ�
									*
									* mem_lower��mem_upper�ֱ�ָ���˵Ͷ˺͸߶��ڴ�Ĵ�С����λ��K��
									* �Ͷ��ڴ���׵�ַ��0���߶��ڴ���׵�ַ��1M��
									* �Ͷ��ڴ��������ֵ��640K��
									* �߶��ڴ��������ֵ�����ֵ��ȥ1M����������֤�����ֵ��
									*/
	uint32_t mem_lower;
	uint32_t mem_upper;

	uint32_t boot_device;           // ָ������������ĸ�BIOS�����豸�����OSӳ��
	uint32_t cmdline;               // �ں�������
	uint32_t mods_count;            // boot ģ���б�
	uint32_t mods_addr;

	/**
	* ELF ��ʽ�ں�ӳ���sectionͷ��
	* ����ÿ��Ĵ�С��һ���м����Լ���Ϊ�����������ַ�����
	*/
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;

	/**
	* ��������ָ��������BIOS�ṩ���ڴ�ֲ��Ļ������ĵ�ַ�ͳ���
	* mmap_addr�ǻ������ĵ�ַ��mmap_length�ǻ��������ܴ�С
	* ��������һ�����߶������Ĵ�С/�ṹ�� mmap_entry_t ���
	*/
	uint32_t mmap_length;
	uint32_t mmap_addr;

	uint32_t drives_length;         // ָ����һ���������ṹ�������ַ       
	uint32_t drives_addr;           // ָ����һ������������ṹ�Ĵ�С
	uint32_t config_table;          // ROM ���ñ�
	uint32_t boot_loader_name;      // boot loader ������
	uint32_t apm_table;             // APM ��
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
} __attribute__((packed)) multiboot_t;

/**
* size����ؽṹ�Ĵ�С����λ���ֽڣ������ܴ�����Сֵ20
* base_addr_low��������ַ�ĵ�32λ��base_addr_high�Ǹ�32λ��������ַ�ܹ���64λ
* length_low���ڴ������С�ĵ�32λ��length_high���ڴ������С�ĸ�32λ���ܹ���64λ
* type����Ӧ��ַ��������ͣ�1�������RAM������������ֵ����������
*/
typedef
struct mmap_entry_t {
	uint32_t size;          // ���� size �ǲ��� size ��������Ĵ�С
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint32_t type;
} __attribute__((packed)) mmap_entry_t;

// type����Ӧ��ַ��������ͣ�1�������RAM������������ֵ����������
#define MULTIBOOT_TYPE_RAM  1

// ����ȫ�ֵ� multiboot_t * ָ��
// �ں�δ������ҳ����ǰ�ݴ��ָ��
extern multiboot_t *mboot_ptr_tmp;

// �ں�ҳ�������ָ��
extern multiboot_t *glb_mboot_ptr;

#endif  // INCLUDE_MULTIBOOT_H_
