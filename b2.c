#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 100
typedef struct Node {
    char english[MAX_LEN];
    char vietnamese[MAX_LEN];
    struct Node* left;
    struct Node* right;
} Node;
Node* createNode(const char* eng, const char* viet) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->english, eng);
    strcpy(newNode->vietnamese, viet);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
Node* insert(Node* root, const char* eng, const char* viet) {
    if (root == NULL) {
        return createNode(eng, viet);
    }
    int cmp = strcmp(eng, root->english);
    if (cmp < 0) {
        root->left = insert(root->left, eng, viet);
    } else if (cmp > 0) {
        root->right = insert(root->right, eng, viet);
    } else {
        printf("Tu nay da ton tai!\n");
    }
    return root;
}
Node* search(Node* root, const char* eng) {
    if (root == NULL || strcmp(eng, root->english) == 0) {
        return root;
    }
    if (strcmp(eng, root->english) < 0) {
        return search(root->left, eng);
    }
    return search(root->right, eng);
}
void displayPostorder(Node* root) {
    if (root == NULL)
        return;
    displayPostorder(root->left);
    displayPostorder(root->right);
    printf("%s: %s\n", root->english, root->vietnamese);
}
void freeTree(Node* root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
int main() {
    Node* root = NULL;
    int choice;
    char eng[MAX_LEN], viet[MAX_LEN];
    do {
        printf("\n------ DICTIONARY MANAGER ------\n");
        printf("1. THEM TU\n");
        printf("2. TRA NGHIA\n");
        printf("3. HIEN THI DANH SACH TU\n");
        printf("4. THOAT\n");
        printf("Nhap lua chon: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                printf("Nhap tu tieng anh: ");
                fgets(eng, MAX_LEN, stdin);
                eng[strcspn(eng, "\n")] = 0;
                printf("Nhap nghia tieng viet: ");
                fgets(viet, MAX_LEN, stdin);
                viet[strcspn(viet, "\n")] = 0;
                root = insert(root, eng, viet);
                printf("Da them tu moi thanh cong!\n");
                break;
            case 2:
                printf("Nhap tu can tra: ");
                fgets(eng, MAX_LEN, stdin);
                eng[strcspn(eng, "\n")] = 0;
                Node* result = search(root, eng);
                if (result != NULL) {
                    printf("Nghia cua tu '%s': %s\n", eng, result->vietnamese);
                } else {
                    printf("Khong tim thay  '%s' trong tu dien!\n", eng);
                }
                break;
            case 3:
                if (root == NULL) {
                    printf("Tu dien trong!\n");
                } else {
                    printf("\nDanh sach tu (Postorder):\n");
                    displayPostorder(root);
                }
                break;
            case 4:
                printf("Dang thoat chuong trinh...\n");
                freeTree(root);
                break;
            default:
                printf("Lua chon khong hop le!\n");
        }
    } while (choice != 4);
    return 0;
}