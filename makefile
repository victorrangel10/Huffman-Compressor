c:
	gcc compactador.c listaArvores.c arvore.c bitmap.c -o compactador
d:
	gcc descompactador.c listaArvores.c arvore.c bitmap.c -o descompactador

runC:
	 --leak-check=full ./compactador $(f)

runD:
	 --leak-check=full ./descompactador $(f)
