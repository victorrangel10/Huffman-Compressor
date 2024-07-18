<h1 align="center">
	Compactador Huffman
</h1>
<h6 align="center">
  João Loss
  ·
  Victor Rangel
</h6>

# Índice 

* [Introdução](#intro)
* [Implementação](#implementacao)
* [Conclusão](#conclusao)
* [Bibliografia](#biblio)
* [Funcionamento](#func)

<a id="intro"></a>

## Introdução
O compactador Huffman é uma aplicação com o intuito de, a partir de um arquivo de entrada, gerar um arquivo binário de tamanho inferior utilizando o algoritmo de codificação de Huffman.

## Funcionamento

### Compactador
A compactação ocorre em duas etapas. Primeiramente os dados presentes no arquivo de entrada são lidos com o objetivo de calcular a frequência (peso) de cada caractere. Com base nos pesos, é criada a árvore binária ótima de Huffman e, a partir dessa, a tabela de codificação, que contém os novos códigos dos caracteres (que representam o caminho na árvore até o nó folha onde o caractere em questão está presente). Gerada a árvore, ela é escrita no cabeçalho do arquivo binário juntamente com algumas outras informações úteis para descompactação. Em seguida, o arquivo de entrada é lido uma segunda vez, etapa onde ocorre a codificação dos dados e escrita, de megabyte em megabtye, no arquivo binário.

### Descompactador
Assim como na compactação, a descompactação ocorre em duas etapas. Primeiramente são lidos os metadados e a árvore binária criada no momento da compactação, a partir da qual a tebela de codificação é reconstruída. Uma vez reconstruída, a decodificação é iniciada, sendo feita de megabyte em megabyte até que a quantidade de caracteres decodificados seja igual à quantidade de caracteres do arquivo original.

### Entrada
O nome do arquivo a ser compactado ou descompactado deve ser passado como parâmetro na chamada do programa.

### Saída
O arquivo compactado ou descompactado gerado a partir do arquivo de entrada.

<a id="implementacao"></a>

## Implementação

O programa foi implementado utilizando conceitos como: listas simplesmente encadeadas com sentinelas, alocação dinâmica de
memória e tipos abstratos de dados utilizando a linguagem C. 

A imagem que segue ilustra o modo como o programa organiza os usuários, as amizades, as playlists e as músicas após a leitura dos arquivos de entrada.

![image](https://github.com/jpploss/playED2.0/assets/130004595/0b92598d-c163-4c44-a5f6-09b7fdf4025f)

### Visão Geral das Bibliotecas
Para fins de modularização e abstração, foram criados três TADs opacos (tUsuario, tPlaylist e tMusica) e três tipos de listas opacas (uma para cada tipo - tListaUsuario, tListaPlaylist e tListaMusica), permitindo, assim, não só a criação e interação com os tipos e seus atributos, mas o agrupamento desses em listas (parte fundamental para o desenvolvimento do programa). Além disso, foi criada uma lista genérica e também opaca (tLista) para realizar operações básicas com o intuito de facilitar algumas partes do código. Todos os tipos e listas foram documentados e detalhas nos respectivos arquivos headers usando o padrão Doxygen.</br>
Segue abaixo um resumos dos TADs e suas listas.

<details>
<summary>tMusica</summary>
	
* **Criação e destruição:** `criaMusica` cria uma música a partir de uma descrição fornecida. `destroiMusica` desaloca a memória utilizada por uma música.

* **Acesso a informações:** `retornaCantorMusica` retorna o cantor ou banda de uma música. `retornaDescricaoMusica` retorna a descrição completa de uma música.

* **Verificação:** `ehMesmoCantor` verifica se duas músicas têm o mesmo cantor ou banda. `ehCantorDaMusica` verifica se um dado cantor ou banda é o responsável por uma música.

* **Impressão:** `imprimeMusica` imprime a descrição de uma música.
</details>

<details>
<summary>tListaMusica</summary>
	
* **Criação e destruição:** `criaListaMusicas` cria uma lista de músicas vazia. `destroiListaMusicas` libera a memória da lista. `destroiListaMusicasEMusicas` libera tanto a lista quanto as músicas.

* **Manipulação de músicas:** `insereFinalListaMusicas` adiciona uma música ao final da lista. `removeMusica` remove uma música específica.

* **Consulta:** `achaMusicaPorDescricao` busca uma música por descrição. `retornaQuantidadeMusicasEmComum` conta músicas em comum entre duas listas. `retornaTamanhoListaMusicas` retorna o número de músicas da lista. `retornaPrimeiraMusicaLista` e `retornaProximaMusicaLista` acessam músicas em determinadas posições. `retornaCantorDaPrimeiraMusica` obtém o cantor da primeira música.

* **Operações em listas:** `adicionaListaMusicas2EmListaMusicas1` combina duas listas sem repetição. `retornaMusicasMesmoCantor` cria uma nova lista com músicas de um cantor específico.
</details>


<details>
<summary>tPlaylist</summary>
	
* **Criação e destruição:** `criaPlaylist` cria uma playlist vazia a partir de um nome fornecido. `destroiApenasPlaylist` libera a memória da playlist sem destruir as músicas. `destroiPlaylistEMusicas` libera a memória da playlist e destrói as músicas contidas nela.

* **Manipulação de músicas:** `insereMusicaPlaylist` adiciona uma música à playlist.

* **Consulta:** `retornaListaMusicas` retorna a lista de músicas de uma playlist. `retornaQuatidadeMusicasPlaylist` retorna a quantidade de músicas em uma playlist. `retornaNomePlaylist` retorna o nome da playlist.

* **Operações em playlists:** `retornaPlaylistCantorMusica1` cria uma nova playlist com músicas do mesmo cantor/banda da primeira música da playlist original (removendo essas músicas da playlist original).

* **Impressão:** `imprimePlaylist` imprime o nome da playlist e a quantidade de músicas.
</details>
<details>
<summary>tListaPlaylist</summary>
	
* **Criação e destruição:** `criaListaPlaylists` cria uma lista de playlists vazia. `destroiListaPlaylists` libera a memória da lista. `destroiListaPlaylistsEPlaylists` libera tanto a lista quanto as playlists.
	
* **Manipulação de playlists:** `insereFinalListaPlaylists` adiciona uma playlist ao final da lista. `removePlaylist` remove uma playlist específica. `separaPorAutoresPlaylist` separa uma playlist por autores, criando uma lista de playlists.

* **Consulta:** `achaPlaylistPorNome` busca uma playlist por nome. `retornaQuantidadeSimilaridadesPlaylists` conta músicas em comum entre duas listas de playlists. `retornaTamanhoListaPlaylists` retorna o número de playlists na lista. `retornaPrimeiraPlaylistLista` e `retornaProximaPlaylistLista` acessam playlists específicas.

* **Operações em listas:** `uneListaPlaylistsEmPlaylist` une músicas de todas as playlists em uma única playlist sem repetições.

* **Impressão:** `imprimeListaPlaylists` imprime todas as playlists da lista.
</details>

<details>
<summary>tUsuario</summary>

* **Criação e destruição:** `criaUsuario` cria um usuário a partir de um nome fornecido, sem amigos e playlists. `destroiUsuario` libera a memória do usuário, destruindo suas playlists, mas não seus amigos.

* **Manipulação de amizades:** `criaAmizade` estabelece uma amizade recíproca entre dois usuários, adicionando cada um à lista de amigos do outro.

* **Manipulação de playlists:**  `adicionaPlaylistUsuario`  insere uma playlist na lista de playlists do usuário. `refazPlaylistsUsuarioPorCantor` reorganiza as playlists do usuário por cantor/banda, criando novas playlists para cada cantor/banda e nomeando-as adequadamente.

* **Consulta:** `retornaAmigosUsuario` retorna a lista de amigos de um usuário. `retornaListaPlaylistsUsuario` retorna a lista de playlists de um usuário. `retornaNomeUsuario` retorna o nome do usuário.

* **Impressão:** `imprimeUsuario` imprime informações sobre o usuário: nome, quantidade de amigos e quantidade de playlists.
</details>

<details>
<summary>tListaUsuario</summary>

* **Criação e destruição:** `criaListaUsuarios` cria uma lista de usuários vazia. `destroiListaUsuarios` libera a memória da lista, sem destruir os usuários. `destroiListaUsuariosEUsuarios` libera tanto a lista quanto os usuários.

* **Manipulação de usuários:** `insereFinalListaUsuarios` adiciona um usuário ao final da lista. `removeUsuario` remove um usuário específico.

* **Consulta:** `retornaPrimeiroUsuarioLista` e `retornaProximoUsuarioLista` acessam usuários específicos na lista. `achaUsuarioPorNome` busca um usuário pelo nome. `retornaTamanhoListaUsuarios` retorna o número de usuários na lista.

* **Impressão:** `imprimeListaUsuarios` chama a função `imprimeUsuario` para todos os usuários na lista.

* **Operações em listas:** `reorganizaPlaylistsListaUsuariosPorCantor` reorganiza as playlists de todos os usuários da lista por cantor/banda.
</details>

<a id="func"></a>
## Funcionamento
* Execução do programa:
	01. Garanta que a pasta "Entrada" (organizada de acordo com as especificações acima e exigências dos TADs) esteja no mesmo diretório do arquivo compilado.
  	02. Garanta que o arquivo "makefile" também esteja nesse mesmo diretório.
  	03. No terminal, digite `make` para compilar o código-fonte e apagar as pastas "Saida" e "Merge" (caso existam de execuções anteriores).
  	04. No terminal, digite `make run` para executar o arquivo compilado.

<a id="conclusao"></a>
## Conclusão
A implementação deste trabalho sobre listas encadeadas e alocação dinâmica em C fortaleceu o desenvolvimento de habilidades 
como: trabalho em grupo usando versionamento de código (Git e Github), criação e manipulação de estruturas aprendidas durante o 
início da disciplina Estrutura de Dados (INF15974) e abstração e modularização utilizando TADs opacos.


<a id="biblio"></a>
## Bibliografia
1. CELES, Waldemar; CERQUEIRA, Renato; RANGEL NETTO, José Lucas Mourão. Introdução a estruturas de dados
com técnicas de programação em C. 1. ed. Rio de Janeiro: Campus, 2004. xiv, 294 p.