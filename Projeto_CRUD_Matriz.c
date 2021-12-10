#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count;
char ***livros;


char *getColuna(char *linha, int num)
{
    char *dado;
    for (dado = strtok(linha, ";");
         dado && *dado;
         dado = strtok(NULL, ";\n"))
    {
        if (!--num)
            return dado;
    }
    return NULL;
}

void listaTodos()
{
    
    printf("----------------------------------------------------------------------------------------------\n");
    printf("|   ID   |             Titulo              |   Ano  |      Autor      |    Genero    |   Qtd  |\n");

    for(int i=0; i<count; i++){
        printf("|%-8s|", livros[i][0]);
        printf("%-33s|", livros[i][1]);
        printf("%-8s|", livros[i][2]);
        printf("%-17s|", livros[i][3]);
        printf("%-14s|", livros[i][4]);
        printf("%-8s|\n", livros[i][5]);
    }
    printf("|--------|---------------------------------|--------|-----------------|--------------|--------|\n");
}

void contaLinhas(){
    count = 0;
    FILE *reg = fopen("livros.csv", "r");
    for (char c = getc(reg); c != EOF; c = getc(reg))
        if (c == '\n')
            count = count + 1;
    fclose(reg);
    free(reg);
}

int login(){
    char*** usuarios;
    FILE *reg = fopen("users.csv", "r");
    int linhas = 0;
    for (char c = getc(reg); c != EOF; c = getc(reg)){
        if (c == '\n'){
            linhas = linhas + 1;
        }
    }
    fclose(reg);
    free(reg);  
    reg = fopen("users.csv", "r");   
    usuarios = malloc(linhas * sizeof(char**));
    for(int i=0; i<linhas; i++){
        usuarios[i] = malloc(3 * sizeof(char*));
        for(int j=0; j<3; j++){
            usuarios[i][j] = malloc(50 * sizeof(char));
        }
    }
    char lines[1024];

    for(int i=0; i<linhas; i++){
        fgets(lines, 1024, reg);
        char *linha = strdup(lines);
        usuarios[i][0] = getColuna(linha, 1);
        linha = strdup(lines);
        usuarios[i][1] = getColuna(linha, 2);
        linha = strdup(lines);
        usuarios[i][2] = getColuna(linha, 3);
        linha = strdup(lines);
        free(linha);
    }
    fclose(reg);
    free(reg);

    printf("1) Entrar\n"
            "2) Novo usuario\n");
    int opcao;
    scanf("%d", &opcao);
    fflush(stdin);
    if(opcao == 2){
        linhas++;
        usuarios = realloc(usuarios, linhas * sizeof(char**));
        usuarios[linhas-1] = malloc(3 * sizeof(char*));
        for(int j=0; j<3; j++){
            usuarios[linhas-1][j] = malloc(50 * sizeof(char));
        }
        printf("Digite o email\n");
        scanf("%[^\n]%*c", usuarios[linhas-1][0]);
        int senha;
        printf("Digite a senha numérico\n");
        scanf("%d", &senha);
        fflush(stdin);
        int hash = (((senha * 10) + 15) * 13);
        itoa(hash, usuarios[linhas-1][1], 10);
        usuarios[linhas-1][2] = "0";
        printf("Registrado com sucesso\n");
        FILE *novoReg = fopen("tempuser.csv", "w+");
        for(int i=0; i<linhas; i++){
            fprintf(novoReg, "%s;%s;%s\n", usuarios[i][0], usuarios[i][1], usuarios[i][2]);
        }
    fclose(novoReg);
    remove("./users.csv");
    rename("./tempuser.csv", "users.csv");
    }

    char user[50];
    int userid = -1;
    do{
        printf("Digite o email\n");
        scanf("%[^\n]%*c", user);
        for(int i=0; i<linhas; i++){
            if (strcmp(usuarios[i][0], user) == 0){
                userid = i;
                break;
            }
        }
        if(userid == -1){
            printf("Usuario nao encontrado\n");
        }
    }while (userid == -1);

    int senhaLog;
    while(1){
        printf("Digite a senha\n");
        scanf("%d", &senhaLog);
        fflush(stdin);
        int senhaHash = (((senhaLog * 10) + 15) * 13);
        char senhaText[50];
        itoa(senhaHash, senhaText, 10);
        if (strcmp(usuarios[userid][1], senhaText) == 0){
            char *numb = usuarios[userid][2];
            int acess_level = atoi(numb);
            return acess_level;
        }
        printf("Senha incorreta\n");
    }


}

int insertionSort()
{
    char *atual = malloc(50 * sizeof(char));
    char ***atual_change = malloc(1 * sizeof(char**));
    for(int i=0; i<count; i++){
        atual_change[i] = malloc(7 * sizeof(char*));
        for(int j=0; j<7; j++){
            atual_change[i][j] = malloc(50 * sizeof(char));
        }
    }
    int i = 0, j = 0;
    for(i = 0; i < count; i++)
    {
        atual = livros[i][1];
        atual_change[0] = livros[i];
        j = i;
        while(j > 0 && atual[0] < livros[j-1][1][0])
        {
            livros[j] = livros[j - 1];
            j--;
        }
        livros[j] = atual_change[0];
    }
}

void carregaArray(){
    FILE *reg = fopen("livros.csv", "r");
    livros = malloc(count * sizeof(char**));
    for(int i=0; i<count; i++){
        livros[i] = malloc(7 * sizeof(char*));
        for(int j=0; j<7; j++){
            livros[i][j] = malloc(50 * sizeof(char));
        }
    }
    char linhas[1024];

    for(int i=0; i<count; i++){
        fgets(linhas, 1024, reg);
        char *linha = strdup(linhas);
        livros[i][0] = getColuna(linha, 1);
        linha = strdup(linhas);
        livros[i][1] = getColuna(linha, 2);
        linha = strdup(linhas);
        livros[i][2] = getColuna(linha, 3);
        linha = strdup(linhas);
        livros[i][3] = getColuna(linha, 4);
        linha = strdup(linhas);
        livros[i][4] = getColuna(linha, 5);
        linha = strdup(linhas);
        livros[i][5] = getColuna(linha, 6);
        linha = strdup(linhas);
        free(linha);
    }
    fclose(reg);

    insertionSort();
}

void cadastraNovo()
{
    int continua = 0, qtd = 0;
    printf("Quantos itens dejesa adicionar? ");
    scanf("%d", &qtd);
    int countTemp = count;
    count += qtd;
    livros = realloc(livros, count * sizeof(char**));
    for(int i=countTemp; i<count; i++){
        livros[i] = malloc(6 * sizeof(char*));
        for(int j=0; j<6; j++){
            livros[i][j] = malloc(50 * sizeof(char));
        }
    }
    do
    {
        fflush(stdin);
        char *inscricao[50];
        printf("Digite o ID: ");
        scanf("%[^\n]%*c", inscricao);
        memcpy(livros[countTemp][0], inscricao, 50);
        printf("Digite o Nome do livro: ");
        scanf("%[^\n]%*c", inscricao);
        memcpy(livros[countTemp][1], inscricao, 50);
        printf("Digite o Ano de lancamento do livro: ");
        scanf("%[^\n]%*c", inscricao);
        memcpy(livros[countTemp][2], inscricao, 50);
        printf("Digite o Nome do Autor do livro: ");
        scanf("%[^\n]%*c", inscricao);
        memcpy(livros[countTemp][3], inscricao, 50);
        printf("Digite o genero do livro: ");
        scanf("%[^\n]%*c", inscricao);
        memcpy(livros[countTemp][4], inscricao, 50);
        printf("Digite a quantidade de estoque:\n ");
        scanf("%[^\n]%*c", inscricao);
        memcpy(livros[countTemp][5], inscricao, 50);
        countTemp++;
        continua++;
    } while (continua < qtd);
    insertionSort();
}

void buscaGenero()
{
    char **generos;
    int qtdGenero = 0;
    int jaContem;
    generos = malloc(count * sizeof(char*));
    for(int i=0; i<count; i++){
        generos[i] = malloc(50 * sizeof(char));
    }
    for(int i=0; i<count; i++){
        jaContem = 0;
        for (int j=0; j<qtdGenero; j++){
            if (strcmp(livros[i][4], generos[j]) == 0){
                jaContem = 1;
                break;
            }
        }
        if(jaContem == 0){
            generos[qtdGenero] = livros[i][4];
            qtdGenero++;
        }
    }
    int genero;
    printf("Digido o numero correspondente ao genero desejado:\n");
    for(int i=0; i<qtdGenero; i++){
        printf("%d) %s\n", i+1, generos[i]);
    }
    scanf("%d", &genero);
    genero--;

    printf("----------------------------------------------------------------------------------------------\n");
    printf("|   ID   |             Titulo              |   Ano  |      Autor      |    Genero    |   Qtd  |\n");

        for(int i=0; i<count; i++){
            if (strcmp(livros[i][4], generos[genero]) == 0)
            {
                printf("|%-8s|", livros[i][0]);
                printf("%-33s|", livros[i][1]);
                printf("%-8s|", livros[i][2]);
                printf("%-17s|", livros[i][3]);
                printf("%-14s|", livros[i][4]);
                printf("%-8s|\n", livros[i][5]);
            }
        }
        

    printf("|--------|---------------------------------|--------|-----------------|--------------|--------|\n");
}

void remover()
{
    char id[50];
    fflush(stdin);
    printf("Digite o ID do livro a ser deletado\n");
    scanf("%[^\n]%*c", id);
    for(int i=0; i<count; i++){
        if(strcmp(id, livros[i][0]) == 0){
            for(int j=i+1; j<count; j++){
                livros[i] = livros[j];
                i++;
            }
            count--;
            break;
        }

    }
    insertionSort();
}

int atualiza()
{
    char id[50];
    fflush(stdin);
    printf("Digite o ID do livro a ser alterado\n");
    scanf("%[^\n]%*c", id);
    for(int i=0; i<count; i++){
        if(strcmp(id, livros[i][0]) == 0){
           int opcao;
            printf("Selecione o item a ser alterado (digite o numero da opcao):\n"
                    "1) ID\n"
                    "2) Nome\n"
                    "3) Ano\n"
                    "4) Autor\n"
                    "5) Genero\n"
                    "6) Quantidade\n");
            scanf("%d", &opcao);
            fflush(stdin);
            printf("Insira o novo dado\n");
            scanf("%[^\n]%*c", livros[i][opcao - 1]);
            break;
        }
    }
    insertionSort();
}

void venda(){
    int opcao;
    printf("1) Listar todos os livros\n"
        "2) Filtrar por genero\n"
        "3) Digitar o ID\n");
    scanf("%d", &opcao);
    fflush(stdin);
    if(opcao == 1){
        listaTodos();
    }else if(opcao == 2){
        buscaGenero();
    }
    char id[50];
    printf("Digite o ID do livro\n");
    scanf("%[^\n]%*c", id);
    for(int i=0; i<count; i++){
        if(strcmp(id, livros[i][0]) == 0){
            char *ptr;
            int qtd = strtol(livros[i][5], &ptr, 10);
            if(qtd == 0){
                printf("Sem estoque");
                break;
            }
            qtd--;
            itoa(qtd, livros[i][5], 10);
            char usuario[50];
        }
    }

}

void salvar(){
    FILE *novoReg = fopen("temp.csv", "w+");
    insertionSort();
    for(int i=0; i<count; i++){
        fprintf(novoReg, "%s;%s;%s;%s;%s;%s\n", livros[i][0], livros[i][1], livros[i][2], livros[i][3], livros[i][4], livros[i][5]);
    }
    fclose(novoReg);
    remove("./livros.csv");
    rename("./temp.csv", "livros.csv");
}

int main()
{
    int acesso = login();
    contaLinhas();
    carregaArray();

    int verifica_opcao;
    if(acesso == 1){
        do
        {
            printf("Selecione o que deseja fazer, digite o numero da opcao:\n"
                "1) Verificar Catalogo\n"
                "2) Adicionar itens no catalogo\n"
                "3) Filtrar por genero\n"
                "4) Deletar por ID\n"
                "5) Atualizar livro\n"
                "6) Registrar venda\n"
                "0) Sair\n");
            scanf("%d", &verifica_opcao);

            if (verifica_opcao == 1)
            {
                listaTodos(livros);
            }
            else if (verifica_opcao == 2)
            {
                cadastraNovo();
            }
            else if (verifica_opcao == 3)
            {
                buscaGenero();
            }
            else if (verifica_opcao == 4)
            {
                remover();
            }
            else if (verifica_opcao == 5)
            {
                atualiza();
            }
            else if (verifica_opcao == 6)
            {
                venda();
            }
        } while (verifica_opcao != 0);

        salvar();

    }else{
        do
        {
            printf("Selecione o que deseja fazer, digite o numero da opcao:\n"
                "1) Verificar Catalogo\n"
                "2) Filtrar por genero\n"
                "0) Sair\n");
            scanf("%d", &verifica_opcao);
            if (verifica_opcao == 1)
            {
                listaTodos(livros);
            }
            else if (verifica_opcao == 2)
            {
                buscaGenero();
            }
        }while (verifica_opcao != 0);
    }

}