#ifndef _SYSCALL
#define _SYSCALL

//D�marre l'application num�ro 'appno' (commence � 0)
//P1, P2 et P4 doivent toujours etre � 0;
//Ne fonctionne pas sur l'�mulateur
int StartAnyApp(int P1, int P2, int appno, int P4);

//Ajoute une touche dans le buffer de touches
//Exemple, si vous ajoutez KEY_CTRL_EXE, le prochain GetKey(int*) se comportera comme si
//l'utilisateur avait appuy� sur EXE
//Je ne sais pas a quoi sert 'mode, �a marche avec 0
void KBD_PutKey(unsigned int keycode, int mode);

//Scan les addin de la m�moire de stockage
//Exemple, si votre addin copie un .G1A dans la m�moire de stockage (depuis la carte SD par exemple)
//il faut appeler Register_addin() avant de pouvoir lancer ce .G1A avec StartAnyApp()
void Register_Addin();

//Retourne l'adresse de la VRAM, quelque soit l'OS (fonctionne aussi sur l'�mulateur)
char* VRam_Base();


/***********************************************************************/
/** Les 3 syscalls suivants servent a acceder � la m�moire principale **/
/** Je n'ai pas beaucoup d'informations,                              **/
/** je ne vous conseil pas de les utiliser avant d'en savoir plus.    **/
/***********************************************************************/

//Cherche un �l�ment dans un dossier de la m�moire principale.
//Exemple: "system" pour les programmes BASIC, "@REV2" pour les pictures
int MCS_SearchDirectoryItem(unsigned char* dir, unsigned char* item, char* flags_0, void* item_ptr, int* data_ptr, int* data_length);

//Cr�e un fichier dans un dossier de la m�moire principale et �crit dedans.
//dirtype=1 pour le dossier "system", c'est le vrai nom du dossier des programmes BASIC
int MCS_PutInternalItem(char dirtype, unsigned char* item, int data_len, void* buffer);

//Supprime un �l�ment d'un dossier de la m�moire principale
int MCS_DeleteInternalItem(char dirtype, unsigned char* item);


/******************************************************************/
/** TOUS les syscalls suivants sont tir�s de fxreverse-doc-1.pdf **/
/******************************************************************/

//D�fini la position du curseur, �quivalent � locate(int, int) de fxlib.h
//Retourne 1 si �a a march�, 0 si les coordonn�es sont en dehors de l'�cran
int Cursor_SetPosition(char column, char row);

//D�fini le style du curser (0=normal, 1=SHIFT, 2=ALPHA)
//Retourne 1 si �a a march�, 0 si l'argument n'est pas bon
int Cursor_SetFlashStyle(short int flashstyle);

//D�fini le mode de clignottement du curseur. Si flashmode!=0, le curseur clignotte, si flashmode==0, le curseur ne clignotte plus
void Cursor_SetFlashMode(long flashmode);

//Permet de r�cup�rer des infos sur le style du curseur
//settingsarray contient dans cet order : colonne, ligne, flash style, fhash mode (il faut donc un tableau de 4 int)
//Retourne : on n'en sait rien
unsigned int Cursor_GetSettings(unsigned int settingsarray[]);

//Equivalent de Cursor_SetFlashMode(0);
void Cursor_SetFlashOff();


/*************************************************************************/
/** Les syscalls suivants servent a appeler les applications du syst�me **/
/*************************************************************************/

//Les arguments sont les m�mes que pour AddIn_main, si runMode vaut 0, c'est qu'on appelle l'application depuis une e-activity
//Dans ce cas, stripNum donne le num�ro du strip de l'e-activity
//Pour ex�cuter normalement une application, mettez runMode � 1
void App_CONICS(int runMode, unsigned short stripNum);
void App_DYNA(int runMode, unsigned short stripNum);
void App_EACT(int runMode, unsigned short stripNum);
void App_EQUA(int runMode, unsigned short stripNum);
void App_PRGM(int runMode, unsigned short stripNum);
void App_FINANCE(int runMode, unsigned short stripNum);
void App_GRAPH(int runMode, unsigned short stripNum);
void App_LINK(int runMode, unsigned short stripNum);
void App_MEMORY(int runMode, unsigned short stripNum);
void App_RECUR(int runMode, unsigned short stripNum);
void App_RUNMAT(int runMode, unsigned short stripNum);
void App_STAT(int runMode, unsigned short stripNum);
void App_SYSTEM(int runMode, unsigned short stripNum);


/**********************************************************/
/** Les syscall suivants servent � l'acces au port s�rie **/
/** Le buffer de r�ception fait 1ko,                     **/
/** et le buffer d'envoi fait 256 octets.                **/
/**********************************************************/

//Lit un caract�re du buffer de r�ception et le copie a l'adresse point�e par 'dest'
//Retourne 0 en cas de succes, 1 si le buffer est vide, 3 si la connexion n'est pas �tablie
int Serial_ReadByte(unsigned char *dest);

//Lit 'max' octets du buffer de r�ception et les �crit dans 'dest'
//'size' donne le nombre d'octets lu
//Retourne 0 en cas de succes, 1 si le buffer est vide, 3 si la connexion n'est pas �tablie
int Serial_ReadBytes(unsigned char *dest, int max, short *size);

//Ecrit 'byte' dans le buffer d'envoi
//Retourne 0
int Serial_WriteByte(unsigned char byte);

//Ecrit 'size' octets dans le buffer d'envoi depuis l'adresse 'scr'
//Retourne 0 en cas de succes, 2 si le buffer est trop plein, 3 si la connexion n'est pas �tablie
int Serial_WriteBytes(unsigned char *src, int size);

//FIFO = first in first out
int Serial_WriteByteFIFO(unsigned char byte);

//Retourne la taille des donn�es du buffer de r�ception
int Serial_GetRxBufferSize(void);

//Retourne l'espace disponible dans le buffer d'envoi
int Serial_GetTxBufferFreeCapacity(void);

//Vide le buffer de r�ception
//Retourne 0 en cas de succes, 3 si la connexion n'est pas �tablie
int Serial_ClearReceiveBuffer(void);

//Vide le buffer d'envoi
//Retourne 0
int Serial_ClearTransmitBuffer(void);

//Ouvre et pr�pare l'interface de communication
//Pour plus de d�tails, consulter fxreverse-doc-1.pdf ci joint
int Serial_Open(unsigned char *conf);

//Ferme l'interface de communication et vide les buffers d'envoi et de r�ception
//Si 'mode'==1, la communication est coup�e sans regarder s'il reste des donn�es a transmettre
//Si 'mode'!=1, la fonction ne ferme pas l'interface de communication s'il reste des donn�es a transmettre
//et retourne 5
//Retourne 0 en cas de succes (communication termin�e) et 5 s'il reste des donn�es a transmettre
int Serial_Close(int mode);

//Copie l'octet num�ro 'index' du buffer de r�ception vers 'dest' si 'index' ne d�passe pas les donn�es du buffer
//Retourne 0 en cas de succes, 1 si 'index' d�passe les donn�es du buffer, 3 si la communication n'est pas �tablie
int Serial_Peek(int index, unsigned char *dest);

//R�cup�re le statut de la connexion
//Retourne 1 si la connexion est �tablie, 3 sinon
int Serial_IsOpen(void);

/*********************************************************************/
/** Les syscalls suivants servent a l'acces aux variables Setup     **/
/** Il est pr�cis� dans la doc que ces syscall sont valides         **/
/** pour l'OS 1.2, mais peut etre pas pour les versions sup�rieures **/
/*********************************************************************/

//Lit la variable Setup num�ro 'index'
//Retourne la valeur de la variables Setup num�ro 'index'
char Setup_GetEntry(unsigned int index);

//Ecrit la valeur 'value' dans la variables Setup num�ro 'index'
//Retourne l'adresse du tableau des variables Setup
char *Setup_SetEntry(unsigned int index, char value);

//Retourne l'adresse de la variable Setup num�ro 'index'
char *Setup_GetEntryPtr(unsigned int index);

/*************************************************************************/
/** Les syscalls suivants servent a acceder aux variables               **/
/** utilis�es par les programmes BASIC, A-Z, r, theta et Ans.           **/
/** Les variables A-Z sont repr�sent�es par leur code ASCII (0x41-0x5A) **/
/** r, thete et Ans sont repr�sent�es par les codes 0xCE, 0xCD et 0xC0  **/
/*************************************************************************/

//Lit la variable 'variable' et copie sa valeur dans 'dest'
//'dest' doit etre un tableau de 24 octets
//Retourne le pointeur envoy�, 'dest'
char *Alpha_GetData(char variable, char *dest);

//Ecrit le contenu de 'scr' dans la variable 'variable'
//'scr doit etre un tableau de 24 octets
//Si 'variable' est valide, retourne 0, sinon retourne 'variable'
char Alpha_SetData(char variable, char *src);

//Initialise toutes les variables Alpha, except� Ans
void Alpha_ClearAll(void);

/*************************************************************************/
/**                   Syscall ajoutes par Calliste                      **/
/*************************************************************************/

// If a key is pressed, its matrixcode is put into matrixcode
// and 1 is returned. If no key is currently pressed, 0 is returned.
int Keyboard_GetPressedKey(short*matrixcode);

#endif
