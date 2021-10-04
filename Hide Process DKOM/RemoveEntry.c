#include "Header.h"

NTSTATUS RemoveEntry(PLIST_ENTRY CurrentEntry) {
	PLIST_ENTRY NextListEntry;
	PLIST_ENTRY PreviousListEntry;

	NextListEntry = (PLIST_ENTRY)CurrentEntry->Flink;
	PreviousListEntry = (PLIST_ENTRY)CurrentEntry->Blink;

	PreviousListEntry->Flink = CurrentEntry->Flink;
	NextListEntry->Blink = CurrentEntry->Blink;

	CurrentEntry->Blink = CurrentEntry;
	CurrentEntry->Flink = CurrentEntry;

	return STATUS_SUCCESS;
}