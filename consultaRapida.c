// Busca de livros por ISBN (usando hashing) ou título (busca sequencial)
Livro* buscarLivro(char* chave, int porISBN) {
    if (porISBN) {
        // Busca por ISBN usando hash
        int indice = hashISBN(chave);
        Livro* atual = tabelaLivros[indice];
        while (atual != NULL) {
            if (strcmp(atual->isbn, chave) == 0) {
                return atual;
            }
            atual = atual->prox;
        }
    } else {
        // Busca sequencial por título
        for (int i = 0; i < MAX_TAM; i++) {
            Livro* atual = tabelaLivros[i];
            while (atual != NULL) {
                if (strcmp(atual->titulo, chave) == 0) {
                    return atual;
                }
                atual = atual->prox;
            }
        }
    }
    return NULL; // Não encontrado
}
 
// Busca de usuários por ID (usando hashing) ou nome (busca sequencial)
Usuario* buscarUsuario(char* chave, int porID) {
    if (porID) {
        // Busca por ID usando hash
        int id = atoi(chave);
        int indice = hashID(id);
        Usuario* atual = tabelaUsuarios[indice];
        while (atual != NULL) {
            if (atual->id == id) {
                return atual;
            }
            atual = atual->prox;
        }
    } else {
        // Busca sequencial por nome
        for (int i = 0; i < MAX_TAM; i++) {
            Usuario* atual = tabelaUsuarios[i];
            while (atual != NULL) {
                if (strcmp(atual->nome, chave) == 0) {
                    return atual;
                }
                atual = atual->prox;
            }
        }
    }
    return NULL; // Não encontrado
}
