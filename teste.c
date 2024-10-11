#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void cifra_de_cesar(char* texto, int deslocamento) {
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        char ch = texto[i];
        if (ch >= 'a' && ch <= 'z') {
            texto[i] = (ch - 'a' + deslocamento) % 26 + 'a';
        } else if (ch >= 'A' && ch <= 'Z') {
            texto[i] = (ch - 'A' + deslocamento) % 26 + 'A';
        }
    }
}

void cifra_de_vigenere(char* texto, const char* chave) {
    int len = strlen(texto);
    int chave_len = strlen(chave);
    int i, j = 0; // Declarar i e j fora do loop
    for (i = 0; i < len; i++) {
        char ch = texto[i];
        if (ch >= 'a' && ch <= 'z') {
            texto[i] = (ch + chave[j % chave_len] - 2 * 'a') % 26 + 'a';
            j++;
        } else if (ch >= 'A' && ch <= 'Z') {
            texto[i] = (ch + chave[j % chave_len] - 2 * 'A') % 26 + 'A';
            j++;
        }
    }
}

void gerar_palavras_chave(char** palavras_chave, int num_palavras, int tamanho_max) {
    const char alfabeto[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // Letras minúsculas e maiúsculas
    int i, j; // Declarar i e j fora do loop
    for (i = 0; i < num_palavras; i++) {
        int tamanho = rand() % tamanho_max + 1;
        palavras_chave[i] = malloc(tamanho + 1);
        if (palavras_chave[i] == NULL) {
            perror("Erro ao alocar memória para a palavra-chave");
            exit(1);
        }
        for (j = 0; j < tamanho; j++) { // j declarado fora
            palavras_chave[i][j] = alfabeto[rand() % (sizeof(alfabeto) - 1)];
        }
        palavras_chave[i][tamanho] = '\0';
    }
}

double medir_tempo_execucao(void (*algoritmo)(char*, int), char* texto, int param) {
    clock_t inicio, fim;
    inicio = clock();
    algoritmo(texto, param);
    fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC; // Tempo em segundos
}

void salvar_resultados_csv(const char* nome_arquivo, double tempos[], int num_execucoes) {
    FILE* arquivo = fopen(nome_arquivo, "w");
    int i;
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arquivo, "Execução,Tempo (s)\n");
    for (i = 0; i < num_execucoes; i++) {
        fprintf(arquivo, "%d,%.6f\n", i + 1, tempos[i]);
    }
    fclose(arquivo);
}

void exibir_menu() {
    printf("\nMenu:\n");
    printf("1. Gerar texto aleatório\n");
    printf("2. Executar cifra de César\n");
    printf("3. Executar cifra de Vigenère\n");
    printf("4. Salvar resultados em CSV\n");
    printf("5. Sair\n");
}

int main() {
    int escolha;
    int i;
    char* texto = NULL; // Ponteiro para o texto
    char* palavras_chave[100];
    double tempos[15];
    int deslocamento = 3; // Deslocamento da cifra de César
    int texto_gerado = 0; // Variável para verificar se o texto foi gerado
    char chave_vigenere[100]; // Chave para a cifra de Vigenère

    do {
        exibir_menu();
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1: // Gerar texto aleatório
                printf("Digite o número de caracteres para o texto: ");
                int num_caracteres;
                scanf("%d", &num_caracteres);
                
                // Alocar espaço para o texto baseado na entrada do usuário
                texto = malloc((num_caracteres + 1) * sizeof(char));
                if (texto == NULL) {
                    perror("Erro ao alocar memória para o texto");
                    return 1;
                }

                // Preencher o texto com caracteres aleatórios
                for (i = 0; i < num_caracteres; i++) {
                    texto[i] = 'a' + (rand() % 26); // Gera letras minúsculas aleatórias
                }
                texto[num_caracteres] = '\0'; // Finaliza a string com o caractere nulo
                printf("Texto aleatório gerado com sucesso!\n");
                texto_gerado = 1; // Marcar que o texto foi gerado
                gerar_palavras_chave(palavras_chave, 100, 10); // Gerar palavras-chave
                break;

            case 2: // Executar cifra de César
                if (!texto_gerado) {
                    printf("Erro: Primeiro gere o texto aleatório!\n");
                } else {
                    // Medir tempos
                    for (i = 0; i < 15; i++) {
                        // Copiar o texto original antes de cada execução
                        char* texto_copia = malloc((strlen(texto) + 1) * sizeof(char));
                        strcpy(texto_copia, texto);
                        
                        tempos[i] = medir_tempo_execucao(cifra_de_cesar, texto_copia, deslocamento);
                        free(texto_copia); // Liberar memória da cópia
                    }
                    printf("Cifra de César executada com sucesso!\n");
                }
                break;

            case 3: // Executar cifra de Vigenère
                if (!texto_gerado) {
                    printf("Erro: Primeiro gere o texto aleatório!\n");
                } else {
                    printf("Digite a chave para a cifra de Vigenère: ");
                    scanf("%s", chave_vigenere);

                    // Medir tempos para cifra de Vigenère
                    for (i = 0; i < 15; i++) {
                        // Copiar o texto original antes de cada execução
                        char* texto_copia = malloc((strlen(texto) + 1) * sizeof(char));
                        strcpy(texto_copia, texto);
                        
                        tempos[i] = medir_tempo_execucao(cifra_de_vigenere, texto_copia, chave_vigenere);
                        free(texto_copia); // Liberar memória da cópia
                    }
                    printf("Cifra de Vigenère executada com sucesso!\n");
                }
                break;

            case 4: // Salvar resultados em CSV
                if (!texto_gerado) {
                    printf("Erro: Primeiro execute uma cifra!\n");
                } else {
                    salvar_resultados_csv("resultados_cesar.csv", tempos, 15);
                    printf("Resultados salvos em resultados_cesar.csv com sucesso!\n");
                }
                break;

            case 5: // Sair
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (escolha != 5);

    // Liberar memória
    free(texto);
    for (i = 0; i < 100; i++) {
        free(palavras_chave[i]);
    }

    return 0;
}
