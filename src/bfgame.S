
#ifdef NEED_UNDERSCORE
# define TRANSFORM_SYMBOLS
# define EXPORT_SYMBOL(sym) _ ## sym
#endif

#ifndef TRANSFORM_SYMBOLS

# define GLOBAL(sym) \
  .global sym; \
  sym ##:

# define GLOBAL_FUNC(sym) \
  GLOBAL (sym)

#else

# define GLOBAL(sym) \
  .global EXPORT_SYMBOL (sym); \
  EXPORT_SYMBOL (sym) ##: \
  sym ##:

# define GLOBAL_FUNC(sym) \
  .global sym; \
  GLOBAL (sym)

#endif


.text


/*----------------------------------------------------------------*/
GLOBAL_FUNC (ASM_main)
/*----------------------------------------------------------------*/
		push   %ebx
		push   %ecx
		push   %esi
		push   %edi
		push   %ebp
		sub    $0x10,%esp
		mov    %eax,0x4(%esp)

	__main_exit:
		xor    %eax,%eax
		add    $0x10,%esp
		pop    %ebp
		pop    %edi
		pop    %esi
		pop    %ecx
		pop    %ebx
		ret

.data

data_0000:
		.fill   0x100000
