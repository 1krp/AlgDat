typedef struct sequence {
    queue* q;
} sequence;

sequence* create_sequence (tree* tr){
    
    sequence* seq = (sequence*)malloc(sizeof(sequence));
    seq->q = create_queue();

    node* next = tr->root;
    queue* q = create_queue();

    // Köa root i q
    enque(q, next);

    while (q->first != NULL) {

        next = deque(q);
        // Köa första elementet från q till seq->q
        enque(seq->q, next);

        if (next != NULL) {
            if (next->left != NULL)
                enque(q, next->left);
            if (next->right != NULL)
                enque(q, next->right);
        }
    }

    return seq;
}