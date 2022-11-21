/* Estruturas de Dados - Trabalho pratico
FINAL
Professor: Diego P. Rubert
Aluno :  David Gama Serrate
RGA : 2019.1906.037-5

*/

#include <cstdio>
#include <string>
using namespace std;

using std::string;

class No
{
  friend class ArvBinBusca;

public:
  No(const int chave);

  void escreve(const char *sep = "");

private:
  int chave;
  No *pai;
  No *esq;
  No *dir;
};

class ArvBinBusca
{

public:
  ArvBinBusca();

  // construtor de c?pia
  ArvBinBusca(const ArvBinBusca &outro);
  ~ArvBinBusca();
  ArvBinBusca &operator=(const ArvBinBusca &outro); // operador de atribui??o

  void escreve_ordenado(); // escreve em percurso em-ordem
  void escreve();

  No *get_raiz();         // devolve a raiz
  No *busca(int k);       // devolve o ponteiro para um elemento, se achar, ou NULL
  No *minimo();           // devolve o menor elemento da ?rvore
  No *maximo();           // devolve o maior elemento da ?rvore
  No *sucessor(No *x);    // devolve o sucessor de um elemento
  No *predecessor(No *x); // devolve o predecessor de um elemento

  void insere(int chave); // insere um elemento
  bool remove(int chave); // busca e remove um elemento

  void limpa(); // remove todos elementos da ?rvore, liberando a memoria

private:
  No *raiz;

  void escreve_ordenado(No *x); // escreve em percurso em-ordem
  void escreve(const string &prefixo, No *x);

  No *busca(No *x, int k); // Parte recursiva da busca
  No *minimo(No *x);       // Parte recursiva de minimo
  No *maximo(No *x);       // Parte recursiva de maximo

  void insere(No *z);             // Insercao, mas ja recebe pronto o no (nao a chave) a ser inserido
  void transplante(No *u, No *v); // Transplante
  void remove(No *z);             // Recebe o no a ser removido (deve tratar todos os casos e fazer transplante, se necessario)

  void limpa(No *x); // dado um n? x, remove recursivamente elementos abaixo e deleta x

  void copia(const ArvBinBusca &T); // copia uma ?rvore T para a atual a partir da raiz,
                                    // usada no construtor de c?pia e no operador de atribui??o
  void copia(No *dest, No *orig);   // copia um n? e os descendentes recursivamente
};

int main(void)
{
  ArvBinBusca T; // construtor ArvBinBusca()
  int v[] = {10, 25, 0, 16, 20, 9, 15, 6, 14, 7, 18, 12, 22, 19, 3, 13};

  for (const auto &x : v)
    T.insere(x);

  printf("T:\n");
  T.escreve();

  printf("Valores de T em ordem crescente: ");
  T.escreve_ordenado();

  printf("\n");
  No *raiz = T.get_raiz();
  printf("Raiz: ");
  raiz->escreve("\n");

  No *pre = T.predecessor(raiz);
  printf("Predecessor da raiz: ");
  pre->escreve("\n");
  No *suc = T.sucessor(raiz);
  printf("Sucessor da raiz: ");
  suc->escreve("\n");
  printf("Sucessor do predecessor da raiz (== raiz): ");
  T.sucessor(pre)->escreve("\n");
  printf("Predecessor do sucessor da raiz (== raiz): ");
  T.predecessor(suc)->escreve("\n");

  No *minimo = T.minimo();
  No *maximo = T.maximo();
  printf("M?nimo: ");
  minimo->escreve("\n");
  printf("M?ximo: ");
  maximo->escreve("\n");

  T.remove(0);  // Caso 1
  T.remove(13); // Caso 2
  T.remove(10); // Caso 3b + 3a

  printf("T after remove 0 (Caso 1), 13 (Caso 2), 10 (Caso 3b + 3a): \n");
  T.escreve();
  printf("\n");

  ArvBinBusca T2 = (T); // construtor de c?pia
  printf("T2:\n");
  T2.escreve();

  T2.insere(30);

  printf("T2 after insert of 30:\n");
  T2.escreve();

  ArvBinBusca T3 = T; // construtor de c?pia
  T3.insere(-8);
  printf("T3 after insert of -8:\n");
  T3.escreve();
  printf("T3:\n");
  T3.escreve();

  T3 = T; // operador de atribui??o
  T3.insere(100);
  printf("T:\n");
  T.escreve();
  printf("T3:\n");
  T3.escreve();
}

//***********************************
//*** IMPLEMENTA??ES DA CLASSE NO ***
//***********************************

No::No(const int chave) : chave(chave),
                          pai(NULL),
                          esq(NULL),
                          dir(NULL)
{
}

void No::escreve(const char *sep)
{
  printf("%2d%s", chave, sep);
}

//********************************************
//*** IMPLEMENTA??ES DA CLASSE ARVBINBUSCA ***
//********************************************
ArvBinBusca::ArvBinBusca()
{
  raiz = NULL;
}

ArvBinBusca::ArvBinBusca(const ArvBinBusca &outro)
{
  copia(outro);
}

ArvBinBusca::~ArvBinBusca()
{
  limpa();
}

ArvBinBusca &ArvBinBusca::operator=(const ArvBinBusca &outro)
{
  limpa();
  copia(outro);
  return *this;
}

void ArvBinBusca::escreve_ordenado()
{
  escreve_ordenado(raiz);
}

/*
Função responsável por escrever
o percurso em ordem
*/
void ArvBinBusca::escreve_ordenado(No *x)
{
  if (x != NULL)
  {
    escreve_ordenado(x->esq);
    x->escreve("  ");
    escreve_ordenado(x->dir);
  }
}

void ArvBinBusca::escreve()
{
  escreve("", raiz);
}

void ArvBinBusca::escreve(const string &prefixo, No *x)
{
  if (x == NULL)
    return;

  bool ehDireito = x->pai and x->pai->dir == x;
  bool temIrmaoEsq = x->pai and x->pai->esq;

  printf(prefixo.c_str());
  printf(ehDireito and temIrmaoEsq ? "+--" : "+--");

  if (x->pai == NULL)
    x->escreve("\n");
  else
    x->escreve(ehDireito ? "d\n" : "e\n");

  escreve(prefixo + (ehDireito and temIrmaoEsq ? "|   " : "    "), x->dir);
  escreve(prefixo + (ehDireito and temIrmaoEsq ? "|   " : "    "), x->esq);
}

No *ArvBinBusca::get_raiz()
{
  return this->raiz;
}

/*
Função responsável por devolver
um ponteiro para um nó com chave k
*/
No *ArvBinBusca::busca(int k)
{
  return busca(raiz, k);
}

No *ArvBinBusca::busca(No *x, int k)
{
  if (x == NULL || k == x->chave)
    return x;

  if (k < x->chave)
    busca(x->esq, k);

  else
    busca(x->dir, k);
}

/*
Função responsavel por encontrar
a menor chave de uma arvore.
*/
No *ArvBinBusca::minimo()
{
  return raiz ? minimo(raiz) : NULL;
}

No *ArvBinBusca::minimo(No *x)
{
  while (x->esq != NULL)
    x = x->esq;

  return x;
}
/*
Função responsavel por encontrar
a maior chave de uma arvore.
*/
No *ArvBinBusca::maximo()
{
  return raiz ? maximo(raiz) : NULL;
}

No *ArvBinBusca::maximo(No *x)
{
  while (x->dir != NULL)
    x = x->dir;

  return x;
}

/*
Função responsável por encontrar
o sucessor de um nó x.
*/
No *ArvBinBusca::sucessor(No *x)
{
  No *y;

  if (x->dir != NULL)
    return minimo(x->dir);

  y = x->pai;

  while (y != NULL && x == y->dir)
  {
    x = y;
    y = y->pai;
  }

  return y;
}

/*
Função responsável por encontrar
 o predecessor de um nó
*/
No *ArvBinBusca::predecessor(No *x)
{
  No *y;

  if (x->esq != NULL)
    return maximo(x->esq);

  y = x->pai;

  while (y != NULL && x == y->esq)
  {
    x = y;
    y = y->pai;
  }

  return y;
}

/*
Função responsavel por inserir
uma chave em uma arvore
*/
void ArvBinBusca::insere(int chave)
{
  No *z = new No(chave);

  z->pai = NULL;
  z->esq = NULL;
  z->dir = NULL;

  insere(z);
}
void ArvBinBusca::insere(No *z)
{
  No *y = NULL;
  No *x = this->raiz;

  while (x != NULL)
  {
    y = x;

    if (z->chave < x->chave)
      x = x->esq;

    else
      x = x->dir;
  }

  z->pai = y;

  if (y == NULL)
    this->raiz = z;

  else
  {
    if (z->chave < y->chave)
      y->esq = z;
    else
      y->dir = z;
  }
}

/*
Função responsável por  substituir dois nós de uma árvore,
onde u recebe v, e v recebe u.
*/
void ArvBinBusca::transplante(No *u, No *v)
{
  if (u->pai == NULL)
    raiz = v;
  else
  {
    if (u == u->pai->esq)
      u->pai->esq = v;
    else
      u->pai->dir = v;
  }
  if (v != NULL)
    v->pai = u->pai;
}

/*
Função responsavel por remover
uma chave z de uma arvore.
*/
bool ArvBinBusca::remove(int chave)
{
  No *z = busca(raiz, chave);

  if (z == NULL)
    return false;

  remove(z);
  delete z;
  return true;
}
void ArvBinBusca::remove(No *z)
{
  No *y;

  if (z->esq == NULL)
    transplante(z, z->dir);

  else
  {
    if (z->dir == NULL)
      transplante(z, z->esq);

    else
    {
      y = minimo(z->dir);

      if (y->pai != z)
      {
        transplante(y, y->dir);
        y->dir = z->dir;
        y->dir->pai = y;
      }

      transplante(z, y);
      y->esq = z->esq;
      y->esq->pai = y;
    }
  }
}

/*
Função responsavel por limpar uma arvore,
afim de liberar mais memoria.
*/
void ArvBinBusca::limpa()
{
  limpa(raiz);
  raiz = NULL;
}
void ArvBinBusca::limpa(No *x)
{
  delete x;
}

/*
Função responsavel por copiar os dados
de uma arvore em uma nova instância.
*/
void ArvBinBusca::copia(const ArvBinBusca &T)
{
  if (T.raiz == NULL)
    raiz = NULL;

  else
  {
    raiz = new No(T.raiz->chave);
    copia(raiz, T.raiz);
  }
}

void ArvBinBusca::copia(No *dest, No *orig)
{
  dest = orig;
  this->raiz = dest;
}
