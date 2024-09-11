# Trabalho Prático 2
# Software Básico
## Prof. Bruno Macchiavello
## Alunos:
- Lucas Corrêa Boaventura - 211038262
- Enzo Zanetti Celentano - 211026495

## Observações
Esse trabalho foi feito em conjunto pelo alunos, porém possui a solução de S_INPUT e S_OUTPUT implementada apenas por Lucas Corrêa (mª 211038262)

### Como rodar
O programa foi feito em dois arquivos com main para gerar o executavel TRADUTOR.
Para rodar, foi feito um makefile para compilar com o GCC as diferentes mains e um codigo de definicao da estrutura de dados. Portanto, utilize 'make' no terminal no diretorio raiz do projeto para montar o programa, gerando os executaveis, na pasta /bin.
Caso não queira utilizar 'make', os comandos utilizados para compilação, ligação e execução encontram-se em /bin/makefile
OBS: Caso não consiga compilar, existe um executável disponível na pasta executaveis (Linux).

### Arquivos teste
Existem alguns arquivos de teste .obj criados em /examples.
Utilizandoo comando  'make run path=./examples/EXEMPLO.obj' pode-se testar o arquivo .s criado, executando-o.

### Sobre I/O
O local o tradutor (seu/caminho/...../tradutor/bin) vai ser onde o tradutor vai sair.
exemplo:
- .\tradutor /your_path/fileName.obj ---> gera -----> /your_path/fileName.s