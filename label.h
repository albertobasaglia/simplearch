#ifndef LABEL_H
#define LABEL_H

#include <stdint.h>
#define LABEL_MAX_LENGTH 20

struct reference {
	uint32_t instruction; // <- which is not complete
	uint32_t index; // <- position of the instruction in the executable
	struct reference* next;
};

void reference_init(struct reference* reference,
		    uint32_t instruction,
		    uint32_t index);

struct label {
	char name[LABEL_MAX_LENGTH];
	uint32_t value;
	int resolved;
	struct reference*
	    reference_head; // <- used only for non resolved labels
	struct label* next;
};

void label_init(struct label* label, const char* name);

void label_destroy(struct label* label);

void label_push_back(struct label* head, struct label* elem);

struct label* label_find_name(struct label* head, const char* name);

struct label* label_new_label(struct label** head,
			      const char* name,
			      int resolved,
			      uint32_t address);

void label_add_reference(struct label* label,
			 uint32_t instruction,
			 uint32_t index);

void reference_destroy(struct reference* reference);

#endif
