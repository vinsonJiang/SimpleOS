; -------------------------------------------------
;       GDTR �� TR ��ز���
;
;       qianyi.lh  2014/11/04  14:15:54
; -------------------------------------------------

[GLOBAL gdt_flush]

gdt_flush:
        mov eax, [esp+4]  ; �������� eax �Ĵ���
        lgdt [eax]        ; ���ص� GDTR [�޸�ԭ��GRUB����]

        mov ax, 0x10      ; �������ݶ�������
        mov ds, ax        ; �������п��Ը��µĶμĴ���
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        jmp 0x08:.flush   ; Զ��ת��0x08�Ǵ����������
                          ; Զ��Ŀ���������ˮ�߲����л�������
.flush:
        ret

[GLOBAL tss_flush]        ; TSS ˢ��
tss_flush:
        mov ax, 0x28      ; TSS ��ȫ�������������ǵ�5��
       		          ; �ʶ� 00101000B ������ 0x28
        ltr ax            ; ���ص� TR �Ĵ���
        ret

