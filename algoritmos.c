#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Função de criptografia com Cifra de César
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

// Função de descriptografia com Cifra de César
void decifra_de_cesar(char* texto, int deslocamento) {
    cifra_de_cesar(texto, 26 - deslocamento); // Deslocamento inverso para descriptografia
}

// Função de criptografia com Cifra de Vigenère
void cifra_de_vigenere(char* texto, const char* chave) {
    int len = strlen(texto);
    int chave_len = strlen(chave);
    int i, j = 0;
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

// Função de descriptografia com Cifra de Vigenère
void decifra_de_vigenere(char* texto, const char* chave) {
    int len = strlen(texto);
    int chave_len = strlen(chave);
    int i, j = 0;
    for (i = 0; i < len; i++) {
        char ch = texto[i];
        if (ch >= 'a' && ch <= 'z') {
            texto[i] = (ch - chave[j % chave_len] + 26) % 26 + 'a';
            j++;
        } else if (ch >= 'A' && ch <= 'Z') {
            texto[i] = (ch - chave[j % chave_len] + 26) % 26 + 'A';
            j++;
        }
    }
}

void gerar_palavras_chave(char** palavras_chave, int num_palavras, int tamanho_max) {
    const char alfabeto[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, j;
    for (i = 0; i < num_palavras; i++) {
        int tamanho = rand() % tamanho_max + 1;
        palavras_chave[i] = malloc(tamanho + 1);
        if (palavras_chave[i] == NULL) {
            perror("Erro ao alocar memória para a palavra-chave");
            exit(1);
        }
        for (j = 0; j < tamanho; j++) {
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

double medir_tempo_execucao_vigenere(void (*algoritmo)(char*, const char*), char* texto, const char* chave) {
    clock_t inicio, fim;
    inicio = clock();
    algoritmo(texto, chave);
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

// Função para força bruta na Cifra de César
void forca_bruta_cesar(char* texto, double tempos[]) {
    char* copia_texto = malloc((strlen(texto) + 1) * sizeof(char));
    int iteracao,deslocamento;
    clock_t inicio, fim;
    for (iteracao = 0; iteracao < 15; iteracao++) {
        inicio = clock();
        for (deslocamento = 1; deslocamento < 26; deslocamento++) {
            strcpy(copia_texto, texto);
            decifra_de_cesar(copia_texto, deslocamento);
        }
        fim = clock();
        tempos[iteracao] = (double)(fim - inicio) / CLOCKS_PER_SEC; // Tempo em segundos
    }
    free(copia_texto);
    salvar_resultados_csv("resultados_forca_bruta_cesar.csv", tempos, 15);
}

// Função para força bruta na Cifra de Vigenère (chaves pequenas de exemplo)
void forca_bruta_vigenere(char* texto, int max_len_chave, double tempos[]) {
    const char alfabeto[] = "abcdefghijklmnopqrstuvwxyz";
    int alfabeto_len = strlen(alfabeto);
    char* copia_texto = malloc((strlen(texto) + 1) * sizeof(char));
    char* chave_tentativa = malloc((max_len_chave + 1) * sizeof(char));
    int len_chave,i,j, iteracao;

    for (iteracao = 0; iteracao < 15; iteracao++) {
        clock_t inicio, fim;
        inicio = clock();
        for (len_chave = 1; len_chave <= max_len_chave; len_chave++) {
            for (i = 0; i < pow(alfabeto_len, len_chave); i++) {
                int idx = i;
                for (j = 0; j < len_chave; j++) {
                    chave_tentativa[j] = alfabeto[idx % alfabeto_len];
                    idx /= alfabeto_len;
                }
                chave_tentativa[len_chave] = '\0';

                strcpy(copia_texto, texto);
                decifra_de_vigenere(copia_texto, chave_tentativa);
            }
        }
        fim = clock();
        tempos[iteracao] = (double)(fim - inicio) / CLOCKS_PER_SEC; // Tempo em segundos
    }
    free(copia_texto);
    free(chave_tentativa);
    salvar_resultados_csv("resultados_forca_bruta_vigenere.csv", tempos, 15);
}


void exibir_menu() {
    printf("\nMenu:\n");
    printf("1. Gerar texto aleatório\n");
    printf("2. Executar cifra de César\n");
    printf("3. Descriptografar com César\n");
    printf("4. Executar cifra de Vigenère\n");
    printf("5. Descriptografar com Vigenère\n");
    printf("6. Salvar resultados em CSV\n");
    printf("7. Força Bruta César\n");
    printf("8. Força Bruta Vigenère\n");
    printf("9. Sair\n");
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
            case 3: // Descriptografar com César
                if (!texto_gerado) {
                    printf("Erro: Primeiro gere o texto aleatório!\n");
                } else {
                    // Medir tempos
                    for (i = 0; i < 15; i++) {
                        // Copiar o texto original antes de cada execução
                        char* texto_copia = malloc((strlen(texto) + 1) * sizeof(char));
                        strcpy(texto_copia, texto);

                        tempos[i] = medir_tempo_execucao(decifra_de_cesar, texto_copia, deslocamento);
                        free(texto_copia); // Liberar memória da cópia
                    }
                    printf("Texto descriptografado com sucesso!\n");
                }
                break;
            case 4: // Executar cifra de Vigenère
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

                        tempos[i] = medir_tempo_execucao_vigenere(cifra_de_vigenere, texto_copia, chave_vigenere);
                        free(texto_copia); // Liberar memória da cópia
                    }
                    printf("Cifra de Vigenère executada com sucesso!\n");
                }
                break;
            case 5: // Descriptografar com Vigenère
                if (!texto_gerado) {
                    printf("Erro: Primeiro gere o texto aleatório!\n");
                } else {
                    printf("Digite a chave para a cifra de Vigenère: ");
                    scanf("%s", chave_vigenere);
                    // Medir tempos para descriptografia de Vigenère
                    for (i = 0; i < 15; i++) {
                        // Copiar o texto original antes de cada execução
                        char* texto_copia = malloc((strlen(texto) + 1) * sizeof(char));
                        strcpy(texto_copia, texto);

                        tempos[i] = medir_tempo_execucao_vigenere(decifra_de_vigenere, texto_copia, chave_vigenere);
                        free(texto_copia); // Liberar memória da cópia
                    }
                    printf("Texto descriptografado com sucesso!\n");
                }
                break;
            case 6: // Salvar resultados em CSV
                if (!texto_gerado) {
                    printf("Erro: Primeiro execute uma cifra!\n");
                } else {
                    salvar_resultados_csv("resultados.csv", tempos, 15);
                    printf("Resultados salvos em resultados.csv com sucesso!\n");
                }
                break;
            case 7: // Força Bruta César
                if (!texto_gerado) {
                    printf("Erro: Primeiro gere o texto aleatório!\n");
                } else {
                    forca_bruta_cesar(texto, tempos);
                    printf("Ataque de força bruta (César) executado e resultados salvos em resultados_forca_bruta_cesar.csv\n");
                }
                break;
            case 8: // Força Bruta Vigenère
                if (!texto_gerado) {
                    printf("Erro: Primeiro gere o texto aleatório!\n");
                } else {
                    forca_bruta_vigenere(texto, 3, tempos); // Exemplo com chaves de até 3 caracteres
                    printf("Ataque de força bruta (Vigenère) executado e resultados salvos em resultados_forca_bruta_vigenere.csv\n");
                }
                break;
            case 9: // Sair
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (escolha != 9);

    // Liberar memória
    free(texto);
    for (i = 0; i < 100; i++) {
        free(palavras_chave[i]);
    }

    return 0;
}
