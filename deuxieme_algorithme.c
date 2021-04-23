#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
const  uint32_t hauteur_max_arbre    = 200;
const  uint32_t max_length_of_string =200;
 /*---------------------------------------------------ALGORITHME 2 ------------------------------------------------------------*/
typedef struct maillon{
    struct maillon* suivant;
    char donnee;
    uint32_t* bits;
}maillon;

typedef struct LISTE{
    maillon* tete;
    maillon* queue;
    uint32_t taille;
}LISTE;

typedef struct table_hachage{
    LISTE** tableau_th;
    uint32_t taille;
}table_hachage;

//Le noeud qui contient le caractère
typedef struct noeud{
    char donnee;//le caractère trouver dans le fichier
    struct noeud *gauche,*droite;// Arbre binaire..
    uint32_t frequence;//Fréquence pour chaque caractère
}noeud;

//Structure arbre || la foret est donc un ensemble des noeuds (des petites arbres) 
typedef struct arbre{
    uint32_t taille; 
    noeud** liste; //liste des pointeurs sur les arbres.
}arbre;

//Creation d'une nouvelle liste
LISTE* nouvelle_liste(){
 LISTE *tmp = (LISTE*)malloc(sizeof(LISTE));
 assert(tmp!=NULL);
 tmp->taille = 0;
 tmp->tete = NULL;
 tmp->queue = NULL;
 return tmp;
}

//Table de Hachage pour stocker les bits de chaque element et la donnée sous forme de char
table_hachage* new_tab_hachage(uint32_t taille){
  uint32_t i;
 
 table_hachage *th = malloc( sizeof(table_hachage) );
  assert(th != NULL);
 
 th->tableau_th = (LISTE**)malloc(taille*sizeof(LISTE*));
   assert(th->tableau_th != NULL);
 
  for( i=0; i<taille; i=i+1 )
  {
  th->tableau_th[i] = nouvelle_liste();
  }
 
  th->taille = taille;
  return th;
  }
//fonction de hachage
 uint32_t hachage( table_hachage *th, char d ){
  return d % th->taille; 
 }
 //ajouter en tete de la liste

void add_tete_liste(LISTE* liste, char d,uint32_t* bits){
    assert(liste->tete !=NULL);
    liste->taille +=1;
    maillon *m = (maillon*)malloc(sizeof(maillon));
    m->donnee = d;
    m->bits = bits;
    m->suivant  = liste->tete;
    liste->tete = m;
    free(m);
}
//ajouter en tete de la liste au sein de la table d'hachage
void add_tete_th( table_hachage *th, char d,uint32_t bits ){
//Trouver l'emplacement dans la table pour inserer
uint32_t p = hachage( th, d );
add_tete_liste( th->tableau_th[p], d,&bits);
}
//fonction qui retourne un noeud avec sa frequence et la donnée.
noeud* creer_noeud(char donnee,uint32_t frequence){
    noeud* tmp = (noeud*)calloc(1,sizeof(noeud));
    tmp->gauche = NULL;
    tmp->droite = NULL;
    tmp->donnee = donnee;
    tmp->frequence = frequence;
    return tmp;
}

//Fonction qui Cree un arbre 
arbre* creer_arbre(uint32_t taille_liste){
    arbre* tmp = (arbre*)calloc(1,sizeof(arbre));
    tmp->taille = 0; // arbre vide 
    tmp->liste = (noeud**)calloc(taille_liste,sizeof(noeud*));
    return tmp;
}

//Trier les petites arbre dans la foret 
void trier_noeuds(arbre* arbre, uint32_t indice) 
{  
    //On doit comparer chaque paire 1-2 et 3-4 ainsi de suite
    //N'oublie pas d'appeler avec un indice qui vaut (taille-1)/2
    //Décrementer la boucle avec i ==> (taile-1)/2 
    //Complexité reduite ==> log(k) en insertion
	uint32_t min = indice; 
	uint32_t gauche = 2 * indice + 1; 
	uint32_t droite = 2 * indice + 2; 

	if (gauche < arbre->taille && arbre->liste[gauche]->frequence < arbre->liste[min]->frequence) 
		min = gauche; 
	if (droite < arbre->taille && arbre->liste[droite]->frequence < arbre->liste[min]->frequence) 
		min=droite; 

	if (min != indice) { 
    //echanger les deux valeurs
		//swap_noeud(&arbre->liste[min],&arbre->liste[indice]); 
	noeud* t =arbre->liste[min] ; 
        arbre->liste[min] =arbre->liste[indice]; 
        arbre->liste[indice] = t; 
        //appeler la fonction => on peut faire mieux!    
        trier_noeuds(arbre, min); 
	} 
}

//Fonction qui retourne le noeud  qui contient la frequence min dans la foret
noeud* min_noeud(arbre *arbre){
    noeud* tmp = arbre->liste[0];
    arbre->liste[0] = arbre->liste[arbre->taille-1];
    (arbre->taille) = (arbre->taille)-1;
    //L'indice est 0
    // Pour voir les deux premieres valeurs 1 et 2 dans la liste
    trier_noeuds(arbre,0);
    //Obligatoirement arbre->liste[0] contient le noeud 
    //Dont la fréquence et minimale..
    return tmp;
}
//AFFICHAGE DE L'arbre
 
//Insertion en log(k)
void inserer_arbre(arbre* arbre,  noeud* noeud){ 
//La taille augmente 
//Exemple ==> insertion du premier noeud
// 5 et 9 sont les plus min des noeuds de la foret donc:
// creer le noeud 9+5=14 en frequence
//On doit l'inserer dans la liste
//IMPORTANT!!
//Normalement le nombre de Noeuds et  5 avec
//un element de la liste pointe sur une foret de 3 noeuds
//les 4 restants pointent sur des arbre avec un seul noeud
//donc la somme vaut 6+1=7
//LA COMPELXITE est REDUITE?
//Et bien oui parce que dans ce cas  on aura pas besoin de de parcourir l'integralité de la liste
// Si SIZE=0 => inserer  en liste[0]
// SINON si SIZE=1 =>insertion donc SIZE=2 => enfin i=1
// comparer noeud->frequence avec liste[0]->frequence si inferieur
//arbre->liste[1] = arbre->liste[0];
// ENFIN INSERER le nouveau noeud en arbre->liste[0]; 
	arbre->taille=arbre->taille+1; 
	int16_t i = arbre->taille - 1; 
	while (i>0 && noeud->frequence < arbre->liste[(i - 1)/2]->frequence) { 
		arbre->liste[i] = arbre->liste[(i - 1) / 2]; 
		i = (i - 1) / 2; 
	} 
	arbre->liste[i] = noeud; 
} 

// Construire l'arbre trié a partir de la foret des neouds
void construire_foret(arbre* arbre ){ 
	uint32_t n = arbre->taille - 1;
    int j =(n-1)/2;
	for (int i =0; i <= j; i++) 
    // quand i = 3 ==> automatique la fonction trier noeud
    // exploite 2*i+1 et 2*i+2
    // quand i = 0 ==> 1st indice est 1 et 2nd indice est 2;
	trier_noeuds(arbre, i);
} 

//Si le poiteur de la gauche et l'autre de la droite est null il s'agit d'une feuille.
_Bool est_feuille(noeud* racine){ 
    if(racine->gauche == NULL && racine->droite == NULL)
      return 1;
    else
      return 0; 
} 

//Création d'une foret qui contient l'ensemble des arbres  sous forme de noeuds
arbre* creer_foret(char *tableau, uint32_t *frequences, uint32_t taille){ 
	arbre* arbre = creer_arbre(taille); 
	for (int i = 0; i < taille; ++i){arbre->liste[i] = creer_noeud(tableau[i], frequences[i]);} 
	arbre->taille = taille; 
	construire_foret(arbre);
	return arbre; 
} 

//code prefixe 
noeud* code_prefixe(char *donnee, uint32_t *frequence, uint32_t taille) { 
	noeud *n_gauche, *n_droite, *n_somme;  
	arbre* arbre = creer_foret(donnee, frequence, taille); 
	// tant que le nombre des noeuds different de 1 y a  d'autre à explorer
	while (arbre->taille != 1) { 
		n_gauche = min_noeud(arbre); 
		n_droite = min_noeud(arbre); 
        // La donnée de ce noeud n'est pas interessante parce qu'on va justement tenir compte de la donnée des feuilles.
		n_somme = creer_noeud('@', n_gauche->frequence + n_droite->frequence); 
		n_somme->gauche = n_gauche; 
		n_somme->droite= n_droite; 
		inserer_arbre(arbre, n_somme); 
	} 
    // Retourner Min Noeud retourne automatique la racine c'est le petit noeud qui reste à la fin
	return min_noeud(arbre); 
} 

//Afficher le code associé
void stocker_code(noeud* racine, uint32_t *tableau, uint32_t indice) { 
 //Donner 0 si on explore la gauche
 //1 sinon 
 if(indice > hauteur_max_arbre){
    fprintf(stderr,"Erreur!!!! Vous devez changer la hauteur maximale de l arbre!\n");
     assert(indice <= hauteur_max_arbre);
    }
	if (racine->gauche != NULL) { 
		tableau[indice] = 0; 
		stocker_code(racine->gauche, tableau, indice + 1); 
	}  
	if (racine->droite != NULL) { 
		tableau[indice] = 1; 
		stocker_code(racine->droite, tableau, indice + 1); 
	} 
	//C'est une feuille? OK! AFFICHER Le Caractère et son code associé
	if (est_feuille(racine)) {
        table_hachage *th = new_tab_hachage(indice);
		printf("%c ==> ", racine->donnee);  
        for (int i = 0; i < indice; ++i) {
		printf("%d", tableau[i]);
        add_tete_th(th,racine->donnee,tableau[i]);
        }
	} 
} 
int* calculer_lettres(const char *file){
    FILE* fichierIn = fopen(file,"rb"); 
    // int *count[256] = { 0 };
    int *count = (int*)calloc(256,sizeof(int)); 
    for(int j = 0;j<256;j++){
        count[j]=0;
    }
    // int* out = (int*)malloc(sizeof(int));
        if(fichierIn == NULL){
            fprintf(stderr,"Le fichier de données n'existe pas!\n");
        }else{ 
            int e=0;
          //  char temp;
             while((e=fgetc(fichierIn))){
             //e = fscanf(fichierIn,"%c",&temp);
             if(e == EOF) break;
             count[e]++; 
             } 
        }
      
    return count;
    } 

 //MAIN
int main(int argc, const char *argv[])
{

    printf("----------------------------------HELLO OTMANE----------------------------------------------------------------------------------\n"); 
  printf("----------------------------------Les frequences du fichier en argument sont----------------------------------------------------\n"); 
	uint32_t *frequences=(uint32_t*)malloc(sizeof(uint32_t));
      char *donnees =(char*)malloc(sizeof(char));  
    int i = 0,k=0;
    int *out;
  //  out contient les frequences de chaque caractère selon sa position dans le tableau d'ASCII 
  out =  calculer_lettres(argv[1]);
  for(i=0;i<256;i++){
      if(out[i]!=0){
          frequences[k]=out[i];
          donnees[k] = i;
          printf("%d   ",frequences[k]);
          k++;
      }
  } 
  printf("\n----------------------------------Code_prefixe commence-------------------------------------------------------------------------\n");
    //Remplir un tableau d'après le fichier
	uint32_t size = k;  
    // L'arbre  des codes
	noeud* racine = code_prefixe(donnees, frequences, size); 
    //Afficher le code  en utilisant l'arbre en parametre
	uint32_t *tableau = (uint32_t*)malloc(sizeof(uint32_t));
    uint32_t indice = 0; 
	stocker_code(racine, tableau, indice); 

    free(tableau);
    free(racine);
    return 0;
}
