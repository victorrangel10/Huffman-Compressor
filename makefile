c:
	gcc compactador.c listaArvores.c arvore.c bitmap.c -o compactador
d:
	gcc descompactador.c listaArvores.c arvore.c bitmap.c -o descompactador

runC:
	valgrind --leak-check=full ./compactador $(f)

runD:
	valgrind --leak-check=full ./descompactador $(f)