#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOD (1 << 30)

typedef struct s_state {
    int hash[9];
    unsigned int cnt;
} t_state;

const int adjacency[9][4] = {
    {1, 3, -1, -1}, {0, 2, 4, -1}, {1, 5, -1, -1},
    {0, 4, 6, -1}, {1, 3, 5, 7}, {2, 4, 8, -1},
    {3, 7, -1, -1}, {4, 6, 8, -1}, {5, 7, -1, -1}
};

const int masks_2[] = {3};
const int masks_3[] = {3,5,6,7};
const int masks_4[] = {3,5,6,7,9,10,11,12,13,14,15};

void get_valid_neighbors(int cell_pos, const int hash[9], int neighbors[], int *count) {
    *count = 0;
    for (int d = 0; d < 4; d++) {
        int p = adjacency[cell_pos][d];
        if (p == -1) break;
        if (hash[p] > 0) neighbors[(*count)++] = p;
    }
}

t_state* create_new_state(const t_state *original, int pos, int value) {
    t_state *new_state = malloc(sizeof(t_state));
    memcpy(new_state->hash, original->hash, sizeof(int)*9);
    new_state->hash[pos] = value;
    new_state->cnt = original->cnt;
    return new_state;
}

void apply_mask_and_add_states(
    const t_state *original,
    int pos,
    const int adj[],
    int adj_count,
    const int *masks,
    int mask_size,
    t_state ***nextt,
    int *nextt_size,
    size_t *nextt_capacity
) {
    int any_valid = 0;
    for (int m = 0; m < mask_size; m++) {
        int sum = 0;
        t_state *new_state = create_new_state(original, pos, 0);

        for (int bit = 0; bit < adj_count; bit++) {
            if (masks[m] & (1 << bit)) {
                sum += original->hash[adj[bit]];
                new_state->hash[adj[bit]] = 0;
            }
        }

        if (sum >= 1 && sum <= 6) {
            new_state->hash[pos] = sum;
            if (*nextt_size >= *nextt_capacity) {
                *nextt_capacity *= 2;
                *nextt = realloc(*nextt, sizeof(t_state*) * (*nextt_capacity));
            }
            (*nextt)[(*nextt_size)++] = new_state;
            any_valid = 1;
        } else {
            free(new_state);
        }
    }

    if (!any_valid) {
        t_state *new_state = create_new_state(original, pos, 1);
        if (*nextt_size >= *nextt_capacity) {
            *nextt_capacity *= 2;
            *nextt = realloc(*nextt, sizeof(t_state*) * (*nextt_capacity));
        }
        (*nextt)[(*nextt_size)++] = new_state;
    }
}


void handle_empty_cell(
    const t_state *current_state,
    int pos,
    t_state ***nextt,
    int *nextt_size,
    size_t *nextt_capacity
) {
    int adj[4], adj_count;
    get_valid_neighbors(pos, current_state->hash, adj, &adj_count);

    if (adj_count < 2) {
        t_state *new_state = create_new_state(current_state, pos, 1);
        if (*nextt_size >= *nextt_capacity) {
            *nextt_capacity *= 2;
            *nextt = realloc(*nextt, sizeof(t_state*) * (*nextt_capacity));
        }
        (*nextt)[(*nextt_size)++] = new_state;
    } else {
        const int *masks;
        int mask_size;
        switch(adj_count) {
            case 2: masks = masks_2; mask_size = sizeof(masks_2)/sizeof(int); break;
            case 3: masks = masks_3; mask_size = sizeof(masks_3)/sizeof(int); break;
            case 4: masks = masks_4; mask_size = sizeof(masks_4)/sizeof(int); break;
            default: return;
        }
        apply_mask_and_add_states(current_state, pos, adj, adj_count, masks, mask_size, nextt, nextt_size, nextt_capacity);
    }
}

unsigned long hash_func(int hash[9]) {
    unsigned long h = 0;
    for (int i = 0; i < 9; i++) {
        h = h * 7 + hash[i];
    }
    return h;
}


void state_same(t_state ***nextt, int *nextt_size) {
    unsigned long capacity = 1024;
    t_state **hash_table = calloc(capacity, sizeof(t_state*));

    int new_size = 0;
    for (int i = 0; i < *nextt_size; i++) {
        t_state *s = (*nextt)[i];
        unsigned long h = hash_func(s->hash) % capacity;

        while (hash_table[h] != NULL) {
            if (memcmp(hash_table[h]->hash, s->hash, sizeof(int)*9) == 0) {
                hash_table[h]->cnt += s->cnt;
                free(s);
                break;
            }
            h = (h + 1) % capacity;
        }

        if (hash_table[h] == NULL) {
            hash_table[h] = s;
            (*nextt)[new_size++] = s;
        }
    }

    *nextt_size = new_size;
    free(hash_table);
}

long convert_number(int number[9]) {
    long result=0;
    for(int i = 0; i < 9; i++) {
        result = (result * 10) + number[i];
    }
    return result;
} 

int is_full(int number[9]) {
    for(int i = 0; i < 9; i++) {
        if(number[i] == 0)
            return 0;
    }
    return 1;
}

int main() {
    int depth;
    t_state **content;
    long final_sum = 0;

    content = malloc(sizeof(t_state*));
    content[0] = malloc(sizeof(t_state));
    content[0]->cnt = 1;
    scanf("%d", &depth);
    for(int i = 0; i < 9; i++) 
        scanf("%d", &content[0]->hash[i]);


    int content_size = 1;

    for(int i = 0; i <= depth; i++) {
        size_t nextt_capacity = 1024;
        t_state **nextt = malloc(sizeof(t_state*) * nextt_capacity);
        int nextt_size = 0;

        for (int k = 0; k < content_size; k++) {
            t_state *current = content[k];
            if (i == depth || is_full(current->hash)) {
                long value = convert_number(current->hash);
                final_sum = (final_sum + (value * current->cnt) % MOD) % MOD;
                continue;
            }

            for (int j = 0; j < 9; j++) {
                if (current->hash[j] == 0) {
                    handle_empty_cell(current, j, &nextt, &nextt_size, &nextt_capacity);
                }
            }
        }

        state_same(&nextt, &nextt_size);
        free(content);
        content = nextt;
        content_size = nextt_size;
    }

    printf("%ld\n", final_sum % MOD);
    return 0;
}