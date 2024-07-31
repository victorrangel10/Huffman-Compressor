c:
	gcc compactador.c listaArvores.c arvore.c bitmap.c -o compactador
d:
	gcc descompactador.c listaArvores.c arvore.c bitmap.c -o descompactador

encode:
	./compactador $(f)

decode:
	 ./descompactador $(f)
