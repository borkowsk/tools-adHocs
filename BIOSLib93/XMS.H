/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯

          Modu’ obs’ugi pami‘ci XMS (eXtended Memory Specification)
                        wersja 1.1 dla Turbo C 2.0
                    Napisany przez Krzysztofa Klimczaka
                               1992.09

               Plik deklarujacy potrzebne funkcje i zmienne (XMSC.H)

  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
     Udostepnienie zmiennej XMSError reprezentujacej aktualny status
     pamieci XMS w programie innym modulom.
     UWAGA: nie nalezy zmieniac tej zmiennej.
            Sluzy ona jedynie do odczytu.
     Najczesciej stosowana przy wydruku bledu:
            XMSGetError( XMSError ).
¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

extern XMSError;

/*¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯
               Deklaracje wszystkich dostepnych funkcji.
    UWAGA: modul wymaga zainicjowania do poprawnej pracy funkcja
                       int XMSInit( void );
¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯*/

int      XMSInit( void );
int      XMMPresent( void );
unsigned GetXMSVersion( void );
unsigned GetXMMVersion( void );
unsigned XMSMemAvail( void );
unsigned XMSMaxAvail( void );
unsigned XMSAllocate(unsigned BlockSize);
int      XMSFree(unsigned EMBHandle);
int      MoveToXMS(unsigned EMBHandle, void far *Source, unsigned long BlockLength);
int      MoveFromXMS(unsigned EMBHandle, void far *Destin, unsigned long BlockLength);
int      XMSResize(unsigned EMBHandle, unsigned NewBlockSize);
int      HMAStatus( void );
int      HMAAllocate( unsigned Size );
int      HMAFree( void );
int      A20GlobalEnable( void );
int      A20GlobalDisable( void );
int      A20LocalEnable( void );
int      A20LocalDisable( void );
int      A20GetStatus( void );
unsigned long LockEMB( unsigned EMBHandle );
int      UnlockEMB( unsigned EMBHandle );
unsigned EMBGetSize( unsigned EMBHandle );
int      EMBGetLock( unsigned EMBHandle );
int      EMBGetAvailHands( unsigned EMBHandle );
unsigned UMBAllocate( unsigned ParSize, unsigned *UMBSegment);
int      UMBFree( unsigned UMBSegment);
char *   XMSGetError( int Error );

