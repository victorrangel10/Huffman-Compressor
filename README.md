<h1 align="center">
	Compactador Huffman
</h1>
<h6 align="center">
  João Loss
  ·
  Victor Rangel
</h6>

# Índice 
- [Índice](#índice)
	- [Introdução](#introdução)
	- [Funcionamento](#funcionamento)
		- [Compactador](#compactador)
		- [Descompactador](#descompactador)
		- [Entrada](#entrada)
		- [Saída](#saída)
	- [Implementação](#implementação)
	- [Funcionamento](#funcionamento-1)
	- [Conclusão](#conclusão)
   	- [Bibliografia](#bibliografia)

## Introdução
A codificação de Huffman é um método de compressão que usa as probabilidades de ocorrência dos símbolos no conjunto de dados a ser comprimido para determinar códigos de tamanho variável para cada símbolo.</br>

Assim como em outros métodos de codificação, os símbolos mais comuns são geralmente representados usando-se menos dígitos que os símbolos que aparecem com menos frequência, garantindo que o tamanho total dos dados seja minimizado, diminuindo o tamanho dos arquivos sem perda de informação.</br> 

Ele foi desenvolvido em 1952 por David A. Huffman que era, na época, estudante de doutorado no MIT, e foi publicado no artigo "A Method for the Construction of Minimum-Redundancy Codes"</br>

O compactador Huffman é uma aplicação que utiliza o algoritmo de codificação de Huffman e uma série de estruturas de dados escrito em C para compactar e descompactar arquivos (como, por exemplo, fotos, vídeos e arquivos de texto).

## Funcionamento

### Compactador
A compactação ocorre em duas etapas. Primeiramente os dados presentes no arquivo de entrada são lidos com o objetivo de calcular a frequência (peso) de cada caractere. Com base nos pesos, é criada a árvore binária ótima de Huffman e, a partir dessa, a tabela de codificação, que contém os novos códigos dos caracteres (que representam o caminho na árvore até o nó folha onde o caractere em questão está presente). Gerada a árvore, ela é escrita no cabeçalho do arquivo binário juntamente com outras informações úteis para descompactação. Em seguida, o arquivo de entrada é lido uma segunda vez, etapa onde ocorre a codificação dos dados e escrita, de megabyte em megabtye, no arquivo binário.

### Descompactador
Assim como na compactação, a descompactação ocorre em duas etapas. Primeiramente são lidos os metadados e a árvore binária criada no momento da compactação, a partir da qual a tebela de codificação é reconstruída. Uma vez reconstruída, a decodificação é iniciada, sendo feita de megabyte em megabyte até que a quantidade de caracteres decodificados seja igual à quantidade de caracteres do arquivo original.

### Entrada
O caminho do arquivo a ser compactado/descompactado (deve ser passado como parâmetro na chamada do programa).

### Saída
O arquivo compactado/descompactado gerado a partir do arquivo de entrada (o arquivo descompactado/compactado será criado na mesma pasta do arquivo original).

## Implementação

O programa foi implementado utilizando conceitos como: árvores binárias, listas simplesmente encadeadas com sentinelas, alocação dinâmica de
memória e tipos abstratos de dados (TADs) opacos utilizando a linguagem C.

Para fins de modularização as estruturas de dados com suas respectivas funções de criação, manipulação e liberação foram criadas em arquivos separados (detalhes sobre as funções podem ser vistos nos headers). Além disso foi fornecido pela professora o TAD bitmap, cujo objetivo é facilitar a manipulação de informações a nível de bits.

## Funcionamento
* Compactador
	1.  Digite `make c` no terminal para compilar o compactador.
	2.  Para executar o programa com o valgrind, digite `make runC f=<caminho_do_arquivo>`.
	3.  Para executar o programa sem o valgrind, digite `./compactador <caminho_do_arquivo>`.

		* Substitua <caminho_do_arquivo> pelo caminho do arquivo a ser compactado.

	* Resultado: `<caminho_do_arquivo>.comp` (arquivo compactado).

* Descompactador
	1.  Digite `make d` no terminal para compilar o descompactador.
	2.  Para executar o programa com o valgrind digite `make runD f=<caminho_do_arquivo>`.
	3.  Para executar o programa sem o valgrind digite `./descompactador <caminho_do_arquivo>`.
   
		* Substitua <caminho_do_arquivo> pelo caminho do arquivo a ser descompactado.

	* Resultado: `<caminho_do_arquivo> (descompactado)` (arquivo descompactado).
  
* Obs.: utilizar o valgrind aumenta significativamente o tempo de execução.

## Conclusão
A implementação deste trabalho sobre árvores binárias, listas encadeadas e alocação dinâmica em C fortaleceu o desenvolvimento de habilidades 
como: trabalho em grupo usando versionamento de código (Git e Github), criação e manipulação de estruturas de dados aprendidas durante a disciplina Estrutura de Dados (INF15974) e abstração/modularização utilizando TADs opacos.

## Bibliografia
CELES, Waldemar; CERQUEIRA, Renato; RANGEL NETTO, José Lucas Mourão. Introdução a estruturas de dados
com técnicas de programação em C. 1. ed. Rio de Janeiro: Campus, 2004. xiv, 294 p.</br>
D. A. Huffman, "A Method for the Construction of Minimum-Redundancy Codes," in Proceedings of the IRE, vol. 40, no. 9, pp. 1098-1101, Sept. 1952, doi: 10.1109/JRPROC.1952.273898.
