#include "label.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void label_push_back(struct label* head, struct label* elem)
{
	while (head->next != NULL) {
		head = head->next;
	}
	head->next = elem;
}

void label_init(struct label* label, const char* name)
{
	label->next = NULL;
	strncpy(label->name, name, LABEL_MAX_LENGTH);
	label->value = 0;
	label->resolved = 0;
	label->reference_head = NULL;
}

void label_destroy(struct label* label)
{
	if(label==NULL)
		return;
	label_destroy(label->next);
	reference_destroy(label->reference_head);
	free(label);
}

struct label* label_find_name(struct label* head, const char* name)
{
	while (head != NULL && strncmp(head->name, name, LABEL_MAX_LENGTH)) {
		head = head->next;
	}
	return head;
}

struct label* label_new_label(struct label** head,
			      const char* name,
			      int resolved,
			      uint32_t address)
{
	struct label* label = malloc(sizeof(struct label));
	label_init(label, name);
	label->resolved = resolved;
	if (resolved) {
		label->value = address;
	}
	if (*head == NULL) {
		*head = label;
	} else {
		label_push_back(*head, label);
	}
	return label;
}

void reference_init(struct reference* reference, uint32_t instruction)
{
	reference->instruction = instruction;
	reference->next = NULL;
}

void label_add_reference(struct label* label,
			 uint32_t instruction,
			 uint32_t index)
{
	struct reference* reference = malloc(sizeof(struct reference));
	reference_init(reference, instruction);
	if (label->reference_head == NULL) {
		label->reference_head = reference;
	} else {
		struct reference* ptr = label->reference_head;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = reference;
	}
}

void reference_destroy(struct reference* reference)
{
	if(reference == NULL)
		return;
	reference_destroy(reference->next);
	free(reference);
}