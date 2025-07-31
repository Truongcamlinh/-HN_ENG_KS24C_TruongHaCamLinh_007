#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Request {
    char username[50];
    char issue[200];
    char timestamp[26];
    struct Request* next;
} Request;

typedef struct {
    Request* front;
    Request* rear;
} Queue;

typedef struct {
    Request* top;
} Stack;

void initQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
}

void initStack(Stack* s) {
    s->top = NULL;
}

Request* createRequest(const char* username, const char* issue) {
    Request* newRequest = (Request*)malloc(sizeof(Request));
    strcpy(newRequest->username, username);
    strcpy(newRequest->issue, issue);
    time_t now = time(NULL);
    strftime(newRequest->timestamp, 26, "%Y-%m-%d %H:%M:%S", localtime(&now));
    newRequest->next = NULL;
    return newRequest;
}

void enqueue(Queue* q, Request* req) {
    if (q->rear == NULL) {
        q->front = req;
        q->rear = req;
    } else {
        q->rear->next = req;
        q->rear = req;
    }
}

Request* dequeue(Queue* q) {
    if (q->front == NULL)
        return NULL;
    Request* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    temp->next = NULL;
    return temp;
}

void push(Stack* s, Request* req) {
    req->next = s->top;
    s->top = req;
}

Request* pop(Stack* s) {
    if (s->top == NULL) return NULL;
    Request* temp = s->top;
    s->top = s->top->next;
    temp->next = NULL;
    return temp;
}

void addToHistory(Request** history, Request* req) {
    Request* newReq = createRequest(req->username, req->issue);
    strcpy(newReq->timestamp, req->timestamp);
    newReq->next = *history;
    *history = newReq;
}

void viewRequests(Queue* q) {
    if (q->front == NULL) {
        printf("Khong co yeu cau nao trong hang doi!\n");
        return;
    }
    printf("\nDanh sach yeu cau dang cho:\n");
    Request* current = q->front;
    while (current != NULL) {
        printf("User: %s\nIssue: %s\nTime: %s\n---\n",
               current->username, current->issue, current->timestamp);
        current = current->next;
    }
}

void viewHistory(Request* history) {
    if (history == NULL) {
        printf("Lich su trong!\n");
        return;
    }
    printf("\nLich su xu ly:\n");
    Request* current = history;
    while (current != NULL) {
        printf("User: %s\nIssue: %s\nTime: %s\n---\n",
               current->username, current->issue, current->timestamp);
        current = current->next;
    }
}

void clearHistory(Request** history) {
    while (*history != NULL) {
        Request* temp = *history;
        *history = (*history)->next;
        free(temp);
    }
    printf("Da xoa lich su!\n");
}

void freeMemory(Queue* q, Stack* s, Request** history) {
    while (q->front != NULL) {
        Request* temp = dequeue(q);
        free(temp);
    }
    while (s->top != NULL) {
        Request* temp = pop(s);
        free(temp);
    }
    clearHistory(history);
}

int main() {
    Queue requestQueue;
    Stack undoStack;
    Request* history = NULL;
    char username[50], issue[200];
    int choice;
    initQueue(&requestQueue);
    initStack(&undoStack);
    do {
        printf("\n-------- REQUEST HANDLER --------\n");
        printf("1. CREATE request\n");
        printf("2. PROCESS\n");
        printf("3. UNDO\n");
        printf("4. VIEW\n");
        printf("5. HISTORY\n");
        printf("6. CLEAR HISTORY\n");
        printf("7. THOAT\n");
        printf("Chon chuc nang:");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Nhap username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;
                printf("Nhap noi dung yeu cau: ");
                fgets(issue, sizeof(issue), stdin);
                issue[strcspn(issue, "\n")] = 0;
                Request* newReq = createRequest(username, issue);
                enqueue(&requestQueue, newReq);
                printf("Da tao yeu cau moi!\n");
                break;

            case 2: {
                Request* processedReq = dequeue(&requestQueue);
                if (processedReq != NULL) {
                    push(&undoStack, processedReq);
                    addToHistory(&history, processedReq);
                    printf("Da xu ly yeu cau cua %s\n", processedReq->username);
                } else
                    printf("Khong co yeu cau nao de xu ly!\n");
                break;
            }
            case 3: {
                Request* undoReq = pop(&undoStack);
                if (undoReq != NULL) {
                    enqueue(&requestQueue, undoReq);
                    printf("Da hoan tac yeu cau!\n");
                } else {
                    printf("Khong co yeu cau nao de hoan tac!\n");
                }
                break;
            }
            case 4:
                viewRequests(&requestQueue);
                break;
            case 5:
                viewHistory(history);
                break;
            case 6:
                clearHistory(&history);
                break;
            case 7:
                printf("Dang thoat chuong trinh...\n");
                freeMemory(&requestQueue, &undoStack, &history);
                break;
            default:
                printf("Lua chon khong hop le!\n");
        }
    } while (choice != 7);
    return 0;
}
