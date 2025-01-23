#include <stdio.h>
#include <stdlib.h>

#define MAX_CIDADES 50
#define NOME_ARQUIVO "cidades.txt"

typedef struct {
    int codigo;
    char estado[30];
    char cidade[30];
    long int populacao;
    float pib; // em milhões
    float area; // em km²
    int pontos_turisticos;
} Cidade;

float calcular_densidade_populacional(Cidade cidade) {
    return cidade.populacao / cidade.area;
}

float calcular_pib_per_capita(Cidade cidade) {
    return (cidade.pib * 1000000) / cidade.populacao; // PIB em reais
}

void exibir_carta(Cidade cidade) {
    printf("\n--- Carta da Cidade ---\n");
    printf("Código: %d\n", cidade.codigo);
    printf("Estado: %s\n", cidade.estado);
    printf("Cidade: %s\n", cidade.cidade);
    printf("População: %ld\n", cidade.populacao);
    printf("PIB: R$ %.2f milhões\n", cidade.pib);
    printf("Área: %.2f km²\n", cidade.area);
    printf("Pontos Turísticos: %d\n", cidade.pontos_turisticos);
    printf("Densidade Populacional: %.2f habitantes/km²\n", calcular_densidade_populacional(cidade));
    printf("PIB per Capita: R$ %.2f\n\n", calcular_pib_per_capita(cidade));
}

void salvar_cidades(Cidade cidades[], int num_cidades) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }
    
    for (int i = 0; i < num_cidades; i++) {
        fprintf(arquivo, "%d,%s,%s,%ld,%.2f,%.2f,%d\n",
                cidades[i].codigo,
                cidades[i].estado,
                cidades[i].cidade,
                cidades[i].populacao,
                cidades[i].pib,
                cidades[i].area,
                cidades[i].pontos_turisticos);
    }
    
    fclose(arquivo);
    printf("Cidades salvas com sucesso em %s.\n", NOME_ARQUIVO);
}

int carregar_cidades(Cidade cidades[]) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para carregar.\n");
        return 0;
    }
    
    int num_cidades = 0;
    while (fscanf(arquivo, "%d,%29[^,],%29[^,],%ld,%f,%f,%d\n",
                  &cidades[num_cidades].codigo,
                  cidades[num_cidades].estado,
                  cidades[num_cidades].cidade,
                  &cidades[num_cidades].populacao,
                  &cidades[num_cidades].pib,
                  &cidades[num_cidades].area,
                  &cidades[num_cidades].pontos_turisticos) == 7) {
        num_cidades++;
    }
    
    fclose(arquivo);
    printf("Cidades carregadas com sucesso: %d\n", num_cidades);
    return num_cidades;
}

int comparar_cidades(Cidade c1, Cidade c2, int criterio) {
    switch (criterio) {
        case 1: // Comparar população
            return (c1.populacao > c2.populacao) ? 1 : (c1.populacao < c2.populacao) ? -1 : 0;
        case 2: // Comparar PIB
            return (c1.pib > c2.pib) ? 1 : (c1.pib < c2.pib) ? -1 : 0;
        case 3: // Comparar área
            return (c1.area > c2.area) ? 1 : (c1.area < c2.area) ? -1 : 0;
        case 4: // Comparar pontos turísticos
            return (c1.pontos_turisticos > c2.pontos_turisticos) ? 1 : (c1.pontos_turisticos < c2.pontos_turisticos) ? -1 : 0;
        default:
            return 0;
    }
}

int main() {
    Cidade cidades[MAX_CIDADES];
    int num_cidades = carregar_cidades(cidades);
    char continuar;

    do {
        if (num_cidades >= MAX_CIDADES) {
            printf("Limite de cidades alcançado.\n");
            break;
        }

        Cidade nova_cidade;

        printf("Digite o código da cidade: ");
        scanf("%d", &nova_cidade.codigo);
        printf("Digite o estado: ");
        scanf("%s", nova_cidade.estado);
        printf("Digite o nome da cidade: ");
        scanf("%s", nova_cidade.cidade);
        printf("Digite a população: ");
        scanf("%ld", &nova_cidade.populacao);
        printf("Digite o PIB (em milhões): ");
        scanf("%f", &nova_cidade.pib);
        printf("Digite a área (em km²): ");
        scanf("%f", &nova_cidade.area);
        printf("Digite o número de pontos turísticos: ");
        scanf("%d", &nova_cidade.pontos_turisticos);

        cidades[num_cidades] = nova_cidade;
        num_cidades++;

        printf("Deseja cadastrar outra cidade? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's');

    // Salvar cidades no arquivo
    salvar_cidades(cidades, num_cidades);

    printf("\n--- Cidades Cadastradas ---\n");
    for (int i = 0; i < num_cidades; i++) {
        exibir_carta(cidades[i]);
    }

    // Comparação de cartas
    int cidade1, cidade2, criterio;
    printf("Escolha duas cidades para comparação (0 a %d):\n", num_cidades - 1);
    printf("Cidade 1: ");
    scanf("%d", &cidade1);
    printf("Cidade 2: ");
    scanf("%d", &cidade2);
    
    printf("Escolha o critério de comparação:\n");
    printf("1. População\n");
    printf("2. PIB\n");
    printf("3. Área\n");
    printf("4. Pontos Turísticos\n");
    printf("Critério: ");
    scanf("%d", &criterio);

    int resultado = comparar_cidades(cidades[cidade1], cidades[cidade2], criterio);
    
    if (resultado > 0) {
        printf("%s vence %s no critério escolhido.\n", cidades[cidade1].cidade, cidades[cidade2].cidade);
    } else if (resultado < 0) {
        printf("%s vence %s no critério escolhido.\n", cidades[cidade2].cidade, cidades[cidade1].cidade);
    } else {
        printf("As duas cidades têm valores iguais no critério escolhido.\n");
    }

    return 0;
}