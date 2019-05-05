// This is the CPP runtime support
//
// (C) Copyright 2019 Tony Mason, All Rights Reserved
//
#include <ntddk.h>
#include <sal.h>

// I'm doing some unsavory things here _and_ using kernel annotations for things that are now considered forbidden
// so I have to mask off several warnings.  Some of this is because I'm on the boundary between C++ and Windows OS
// functionality.
#pragma warning(disable:28285 28218 28160 26408 26447)

static const char* default_tag_str = "lksW";
#pragma warning(suppress:26493)
static const ULONG default_tag = *(ULONG*)default_tag_str;

__drv_allocatesMem(Mem)
_When_((PoolType& PagedPool) != 0, _IRQL_requires_max_(APC_LEVEL))
_When_((PoolType& PagedPool) == 0, _IRQL_requires_max_(DISPATCH_LEVEL))
_When_((PoolType& NonPagedPoolMustSucceed) != 0,
	__drv_reportError("Must succeed pool allocations are forbidden. "
		"Allocation failures cause a system crash"))
	_When_((PoolType& (NonPagedPoolMustSucceed |
		POOL_RAISE_IF_ALLOCATION_FAILURE)) == 0,
		_Post_maybenull_ _Must_inspect_result_)
	_When_((PoolType& (NonPagedPoolMustSucceed |
		POOL_RAISE_IF_ALLOCATION_FAILURE)) != 0,
		_Post_notnull_)
	_Post_writable_byte_size_(length)
void *__cdecl malloc(_In_ __drv_strictTypeMatch(__drv_typeExpr) POOL_TYPE pool, size_t length, ULONG tag)
{
	return ExAllocatePoolWithTag(pool, length, tag);
}

__drv_allocatesMem(Mem)
_Post_maybenull_
_Must_inspect_result_
_Post_writable_byte_size_(length)
void *__cdecl malloc(_In_ size_t length)
{
	return malloc(PagedPool, length, default_tag);
}

__drv_allocatesMem(Mem)
_When_((PoolType& PagedPool) != 0, _IRQL_requires_max_(APC_LEVEL))
_When_((PoolType& PagedPool) == 0, _IRQL_requires_max_(DISPATCH_LEVEL))
_When_((PoolType& NonPagedPoolMustSucceed) != 0,
	__drv_reportError("Must succeed pool allocations are forbidden. "
		"Allocation failures cause a system crash"))
	_When_((PoolType& (NonPagedPoolMustSucceed |
		POOL_RAISE_IF_ALLOCATION_FAILURE)) == 0,
		_Post_maybenull_ _Must_inspect_result_)
	_When_((PoolType& (NonPagedPoolMustSucceed |
		POOL_RAISE_IF_ALLOCATION_FAILURE)) != 0,
		_Post_notnull_)
	_Post_writable_byte_size_(length)
void *__cdecl malloc(POOL_TYPE pool, size_t length)
{
	return malloc(pool, length, default_tag);
}

_IRQL_requires_max_(DISPATCH_LEVEL)
void __cdecl free(_In_ __drv_freesMem(Mem)  void *buffer, _In_ ULONG tag)
{
	if (nullptr != buffer) {
		ExFreePoolWithTag(buffer, tag);
	}
}

_IRQL_requires_max_(DISPATCH_LEVEL)
void __cdecl free(_In_ __drv_freesMem(Mem)  void* buffer)
{
	free(buffer, default_tag);
}


void *__cdecl operator new(size_t size)
{
	return malloc(PagedPool, size, default_tag);
}

void *__cdecl operator new(size_t size, POOL_TYPE pool)
{
	return malloc(pool, size, default_tag);
}

void *__cdecl operator new(size_t size, POOL_TYPE pool, ULONG tag)
{
	return malloc(pool, size, tag);
}


void *__cdecl operator new(size_t size, void *buffer)
{
	UNREFERENCED_PARAMETER(size);

	return buffer;
}

void __cdecl operator delete(void* pVoid, ULONG tag)
{
	free(pVoid, tag);
}

void __cdecl operator delete(void *pVoid)
{
	free(pVoid, default_tag);
}

