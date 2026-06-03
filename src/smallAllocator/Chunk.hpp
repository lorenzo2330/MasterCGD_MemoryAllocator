#pragma once

struct Chunk {

	void Print(								//Funzione custom per il debug, stampa lo stato del chunk
		std::size_t blockSize, 
		unsigned char blocks
	);	

	bool Init(								//Funzione per inizializzare il Chunk (invocata da FixedAllocator)
		std::size_t blockSize,				//Dimensione di ogni blocco del chunk
		unsigned char blocks				//Numero di blocchi del chunk (max 256) (unsigned char per risparmiare spazio)
	);

	void* Allocate(							//Funzione per allocare un blocco di memoria (invocata da FixedAllocator) (ret 0 se pieno)
		std::size_t blockSize				//Dimensione del blocco da allocare -> Nota*
	);

	void Deallocate(						//Funzione per deallocare un blocco di memoria (invocata da FixedAllocator)
		void* p,							//Puntatore al blocco da deallocare
		std::size_t blockSize				//Dimensione del blocco da deallocare -> Nota*
	);

	bool Reset(								//Funzione per ripristinare i valori del chunk	//TODO: uguale a init?
		std::size_t blockSize,				//Dimensione di ogni blocco del chunk
		unsigned char blocks				//Numero di blocchi del chunk (max 256) (unsigned char per risparmiare spazio)
	);

	bool Release();							//Rilascia pData al termine dell'esecuzione (invocata da FixedAllocator)

	unsigned char* pData_;					//Puntatore all'inizio dell'area dati del chunk (dove sono allocati i blocchi)
	unsigned char firstAvailableBlock_;		//Indice del primo blocco del chunk disponibile
	unsigned char blocksAvailable_;			//Numero di blocchi del chunk disponibili
};

/*

Struttura del chunk:
|---0---|---1---|---2---|---3---|---4---|--...--|
|--occ--|--occ--|--lib--|--occ--|--lib--|--...--|

chunk.firstAvailableBlock_ = 2
chunk[0] = ...	//Byte occupati
chunk[1] = ...	//Byte occupati
chunk[2] = 4	//Il secondo blocco del chunk è libero, il suo contenuto sarà l'indice del prossimo blocco libero (in questo caso 4)
chunk[3] = ...	//Byte occupati
chunk[4] = x	//Contiene l'indice del prossimo blocco libero

Blocco libero (con unsigned char):
|--indice prossimo blocco libero--|--resto del blocco--|
|--------------1 byte-------------|--size_t - 1 byte---|

Blocco libero (con unsigned short):
|--indice prossimo blocco libero--|--resto del blocco--|
|--------------2 byte-------------|--size_t - 2 byte---|

*/

