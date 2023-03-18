#ifndef PRINTER_H
#define PRINTER_H
#pragma once
#define E_SUCCESS 0
#ifdef __cplusplus
#define PRINTER_API extern "C" __declspec(dllimport)
#else
#define PRINTER_WIN32_SDK_API __declspec(dllimport)
#endif
typedef int(__stdcall*FormatError)(int, int, char*, int, int);
typedef int(__stdcall*PrinterCreator)(void**, const char*);
typedef int(__stdcall*PrinterCreatorS)(char*);
typedef int(__stdcall*PortOpen)(void*, const char*);
typedef int(__stdcall*DriverPortOpen)(void*, const char*);
typedef int(__stdcall*PortClose)(void*);
typedef int(__stdcall*PrinterDestroy)(void*);
typedef int(__stdcall*DirectIO)(void* handle, unsigned char writedata[], unsigned int writenum, unsigned char readdata[], unsigned int readnum, unsigned int* preadednum);

typedef int(__stdcall*TSPL_SelfTest)(void*);
typedef int(__stdcall*TSPL_BitMap)(void* , int, int , int , const char*);
typedef int(__stdcall*TSPL_Setup)(void* , int , int , int , int , int , int, int );
typedef int(__stdcall*TSPL_ClearBuffer)(void* );
typedef int(__stdcall*TSPL_Box)(void* , int , int , int , int , int );
typedef int(__stdcall*TSPL_BarCode)(void* , int , int , int , int , int , int , int , int , const char* );
typedef int(__stdcall*TSPL_QrCode)(void* , int , int , int , int , int , int , int , int , const char* );
typedef int(__stdcall*TSPL_Dmatrix)(void* , int , int , int , int , int , int , int , const char* );
typedef int(__stdcall*TSPL_Text)(void* , int , int , int , int , int , int ,int , const char* );
typedef int(__stdcall*TSPL_Print)(void* , int , int );
typedef int(__stdcall*TSPL_FormFeed)(void* );
typedef int(__stdcall*TSPL_SetTear)(void* , int );
typedef int(__stdcall*TSPL_SetRibbon)(void*, int);
typedef int(__stdcall*TSPL_Offset)(void* , int );
typedef int(__stdcall*TSPL_Direction)(void* , int );


typedef int(__stdcall*TSPL_Feed)(void* , int );
typedef int(__stdcall*TSPL_Home)(void* );
typedef int(__stdcall*TSPL_Learn)(void* );
typedef int(__stdcall*TSPL_GetDllVersion)(void* );
typedef int(__stdcall*TSPL_GetSN)(void* , char* );
typedef int(__stdcall*TSPL_GetPrinterStatus)(void*, int*);
typedef int(__stdcall*TSPL_SetCodePag)(void*, int);

typedef int(__stdcall*TSPL_Reverse)(void*,int,int,int,int);
typedef int(__stdcall*TSPL_GapDetect)(void*, int, int);
typedef int(__stdcall*TSPL_Cut)(void*, int);
typedef int(__stdcall*TSPL_Bold)(void*, int);

extern void* hprinter;


extern FormatError                                  DemFormatError;
extern PrinterCreator                               DemPrinterCreator;
extern PrinterCreatorS                              DemPrinterCreatorS;
extern PrinterDestroy                               DemPrinterDestroy;
extern PortOpen                                     DemPortOpen;
extern DriverPortOpen                               DemDriverPortOpen;
extern PortClose                                    DemPortClose;
extern DirectIO                                     DemDirectIO;

extern TSPL_SelfTest                                DemSelfTest;
extern TSPL_BitMap                                  DemBitMap;
extern TSPL_Setup                                   DemSetup;
extern TSPL_ClearBuffer                             DemClearBuffer;
extern TSPL_Box                                     DemBox;
extern TSPL_BarCode                                 DemBarCode;
extern TSPL_QrCode                                  DemQrCode;
extern TSPL_Dmatrix                                 DemDmatrix;
extern TSPL_Text                                    DemText;
extern TSPL_Print                                   DemPrint;
extern TSPL_FormFeed                                DemFormFeed;
extern TSPL_SetTear                                 DemSetTear;

extern TSPL_SetRibbon                               DemSetRibbon;
extern TSPL_Offset                                  DemOffset;
extern TSPL_Direction                               DemDirection;
extern TSPL_Feed                                    DemFeed;
extern TSPL_Home                                    DemHome;
extern TSPL_Learn                                   DemLearn;

extern TSPL_GetSN                                   DemGetSN;
extern TSPL_GetPrinterStatus                        DemStatus;
extern TSPL_SetCodePag                              DemCodePag;
extern TSPL_Reverse                                 DemReverse;
extern TSPL_GapDetect                               DemGapDetect;
extern TSPL_Cut									    DemCut;
extern TSPL_Bold									DemBold;

#endif // PRINTER_H

