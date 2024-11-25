#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

struct Produto {
    char nome[30];
    float preco;
    int quantidade;
    char status;
};

void cadastrarProduto(FILE *arq) {
    struct Produto prod;
    char confirma;

    printf("\n=== Cadastro de Produto ===\n");
    fflush(stdin);
    printf("Nome do Produto: ");
    fgets(prod.nome, sizeof(prod.nome), stdin);
    strtok(prod.nome, "\n");
    printf("Preço: ");
    scanf("%f", &prod.preco);
    printf("Quantidade: ");
    scanf("%d", &prod.quantidade);
    prod.status = 'A';
    printf("Confirma cadastro? (S/N): ");
    getchar();
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S') {
        fseek(arq, 0, SEEK_END);
        fwrite(&prod, sizeof(struct Produto), 1, arq);
        printf("Produto cadastrado com sucesso!\n");
    } else {
        printf("Cadastro cancelado.\n");
    }
}

void consultarProduto(FILE *arq) {
	setlocale (LC_ALL, "Portuguese");
    struct Produto prod;
    int codigo;

    printf("\n=== Consulta de Produto ===\n");
    printf("Digite o número do produto: ");
    scanf("%d", &codigo);

    if (codigo > 0 && codigo <= tamanho(arq)) {
        fseek(arq, (codigo - 1) * sizeof(struct Produto), SEEK_SET);
        fread(&prod, sizeof(struct Produto), 1, arq);

        if (prod.status == 'A') {
            printf("\nNome: %s\n", prod.nome);
            printf("Preço: R$ %.2f\n", prod.preco);
            printf("Quantidade: %d\n", prod.quantidade);
        } else {
            printf("Produto excluído.\n");
        }
    } else {
        printf("Número inválido!\n");
    }
}

void listarProdutos(FILE *arq) {
	setlocale (LC_ALL, "Portuguese");
    struct Produto prod;
    int i;

    printf("\n=== Lista de Produtos ===\n");
    for (i = 0; i < tamanho(arq); i++) {
        fseek(arq, i * sizeof(struct Produto), SEEK_SET);
        fread(&prod, sizeof(struct Produto), 1, arq);

        if (prod.status == 'A') {
            printf("Produto %d:\n", i + 1);
            printf("  Nome: %s\n", prod.nome);
            printf("  Preço: R$ %.2f\n", prod.preco);
            printf("  Quantidade: %d\n", prod.quantidade);
        }
    }
}

void excluirProduto(FILE *arq) {
	setlocale (LC_ALL, "Portuguese");
    struct Produto prod;
    int codigo;
    char confirma;

    printf("\n=== Excluir Produto ===\n");
    printf("Digite o número do produto: ");
    scanf("%d", &codigo);

    if (codigo > 0 && codigo <= tamanho(arq)) {
        fseek(arq, (codigo - 1) * sizeof(struct Produto), SEEK_SET);
        fread(&prod, sizeof(struct Produto), 1, arq);

        if (prod.status == 'A') {
            printf("Produto encontrado: %s\n", prod.nome);
            printf("Confirma exclusão? (S/N): ");
            getchar();
            scanf("%c", &confirma);

            if (toupper(confirma) == 'S') {
                prod.status = '*';
                fseek(arq, (codigo - 1) * sizeof(struct Produto), SEEK_SET);
                fwrite(&prod, sizeof(struct Produto), 1, arq);
                printf("Produto excluído com sucesso!\n");
            } else {
                printf("Exclusão cancelada.\n");
            }
        } else {
            printf("Produto já excluído.\n");
        }
    } else {
        printf("Número inválido!\n");
    }
}

int tamanho(FILE *arq) {
	setlocale (LC_ALL, "Portuguese");
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(struct Produto);
}

int main() {
	setlocale (LC_ALL, "Portuguese");
    FILE *arq;
    int opcao;

    if ((arq = fopen("produtos.dat", "rb+")) == NULL) {
        if ((arq = fopen("produtos.dat", "wb+")) == NULL) {
            printf("Erro ao abrir o arquivo!\n");
            return 1;
        }
    }

    do {
        printf("\n======= SISTEMA DE PRODUTOS =======\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Consultar Produto\n");
        printf("3. Listar Produtos\n");
        printf("4. Excluir Produto\n");
        printf("5. Sair\n");
        printf("==================================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarProduto(arq);
                break;
            case 2:
                consultarProduto(arq);
                break;
            case 3:
                listarProdutos(arq);
                break;
            case 4:
                excluirProduto(arq);
                break;
            case 5:
                fclose(arq);
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}
