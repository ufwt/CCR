	.data
data_label:
	.extern big_external_data_label,0x400
	.extern small_external_data_label,4
	.comm big_external_common,0x400
	.comm small_external_common,4
	.lcomm big_local_common,0x400
	.lcomm small_local_common,4

# the small symbols should have space allocated in the sbss section
# but this is not yet supported in the assembler, so space is allocated
# in the .bss section and the relocations are not gp-relative. this will
# be updated when gp-relative relocations are added
	.text
	stw	r4,0(r0)
	stw	r4,4(r0)
	stw	r4,0x7fc(r0)
	stw	r4,-0x800(r0)
	stw	r4,0(r5)
	stw	r4,4(r5)
	stw	r4,0x7fc(r5)
	stw	r4,-0x800(r5)
	stw	r4,data_label(r0)
	stw	r4,big_external_data_label(r0)
	stw	r4,small_external_data_label(r0)
	stw	r4,big_external_common(r0)
	stw	r4,small_external_common(r0)
	stw	r4,big_local_common(r0)
	stw	r4,small_local_common(r0)
	stw	r4,data_label+4(r0)
	stw	r4,big_external_data_label+4(r0)
	stw	r4,small_external_data_label+4(r0)
	stw	r4,big_external_common+4(r0)
	stw	r4,small_external_common+4(r0)
	stw	r4,big_local_common+4(r0)
	stw	r4,small_local_common+4(r0)
	stw	r4,data_label-0x800(r0)
	stw	r4,big_external_data_label-0x800(r0)
	stw	r4,small_external_data_label-0x800(r0)
	stw	r4,big_external_common-0x800(r0)
	stw	r4,small_external_common-0x800(r0)
	stw	r4,big_local_common-0x800(r0)
	stw	r4,small_local_common-0x800(r0)
	stw	r4,data_label+0x10000(r0)
	stw	r4,data_label(r5)
	stw	r4,big_external_data_label(r5)
	stw	r4,small_external_data_label(r5)
	stw	r4,big_external_common(r5)
	stw	r4,small_external_common(r5)
	stw	r4,big_local_common(r5)
	stw	r4,small_local_common(r5)
	stw	r4,data_label+4(r5)
	stw	r4,big_external_data_label+4(r5)
	stw	r4,small_external_data_label+4(r5)
	stw	r4,big_external_common+4(r5)
	stw	r4,small_external_common+4(r5)
	stw	r4,big_local_common+4(r5)
	stw	r4,small_local_common+4(r5)
	stw	r4,data_label-0x800(r5)
	stw	r4,big_external_data_label-0x800(r5)
	stw	r4,small_external_data_label-0x800(r5)
	stw	r4,big_external_common-0x800(r5)
	stw	r4,small_external_common-0x800(r5)
	stw	r4,big_local_common-0x800(r5)
	stw	r4,small_local_common-0x800(r5)
	
	stwio	r4,0(r0)
	stwio	r4,4(r0)
	stwio	r4,0x7fc(r0)
	stwio	r4,-0x800(r0)
	stwio	r4,0(r5)
	stwio	r4,4(r5)
	stwio	r4,0x7fc(r5)
	stwio	r4,-0x800(r5)
	stwio	r4,data_label(r0)
	stwio	r4,big_external_data_label(r0)
	stwio	r4,small_external_data_label(r0)
	stwio	r4,big_external_common(r0)
	stwio	r4,small_external_common(r0)
	stwio	r4,big_local_common(r0)
	stwio	r4,small_local_common(r0)
	stwio	r4,data_label+4(r0)
	stwio	r4,big_external_data_label+4(r0)
	stwio	r4,small_external_data_label+4(r0)
	stwio	r4,big_external_common+4(r0)
	stwio	r4,small_external_common+4(r0)
	stwio	r4,big_local_common+4(r0)
	stwio	r4,small_local_common+4(r0)
	stwio	r4,data_label-0x800(r0)
	stwio	r4,big_external_data_label-0x800(r0)
	stwio	r4,small_external_data_label-0x800(r0)
	stwio	r4,big_external_common-0x800(r0)
	stwio	r4,small_external_common-0x800(r0)
	stwio	r4,big_local_common-0x800(r0)
	stwio	r4,small_local_common-0x800(r0)
	stwio	r4,data_label+0x10000(r0)
	stwio	r4,data_label(r5)
	stwio	r4,big_external_data_label(r5)
	stwio	r4,small_external_data_label(r5)
	stwio	r4,big_external_common(r5)
	stwio	r4,small_external_common(r5)
	stwio	r4,big_local_common(r5)
	stwio	r4,small_local_common(r5)
	stwio	r4,data_label+4(r5)
	stwio	r4,big_external_data_label+4(r5)
	stwio	r4,small_external_data_label+4(r5)
	stwio	r4,big_external_common+4(r5)
	stwio	r4,small_external_common+4(r5)
	stwio	r4,big_local_common+4(r5)
	stwio	r4,small_local_common+4(r5)
	stwio	r4,data_label-0x800(r5)
	stwio	r4,big_external_data_label-0x800(r5)
	stwio	r4,small_external_data_label-0x800(r5)
	stwio	r4,big_external_common-0x800(r5)
	stwio	r4,small_external_common-0x800(r5)
	stwio	r4,big_local_common-0x800(r5)
	stwio	r4,small_local_common-0x800(r5)