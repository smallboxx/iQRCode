/***********************************
*  File: printer.cpp
*  Author: Mao Yu Meng
*  Time: 2022-10-20
************************************/

#include "mainwindow.h"
void MainWindow::initPrinter()
{
    QLibrary m_hDll("./TSPL_SDK.dll");
    m_hDll.load();
    DemPrinterCreator=(PrinterCreator)m_hDll.resolve("PrinterCreator");
    DemPortOpen =(PortOpen)m_hDll.resolve("PortOpen");
    DemStatus =(TSPL_GetPrinterStatus)m_hDll.resolve("TSPL_GetPrinterStatus");
    DemPortClose = (PortClose)m_hDll.resolve("PortClose");
    DemPrinterDestroy = (PrinterDestroy)m_hDll.resolve("PrinterDestroy");
    DemSetup = (TSPL_Setup)m_hDll.resolve("TSPL_Setup");
    DemClearBuffer = (TSPL_ClearBuffer)m_hDll.resolve("TSPL_ClearBuffer");
    DemBarCode = (TSPL_BarCode)m_hDll.resolve("TSPL_BarCode");
    DemPrint = (TSPL_Print)m_hDll.resolve("TSPL_Print");
    DemText = (TSPL_Text)m_hDll.resolve("TSPL_Text");
    DemDirection = (TSPL_Direction)m_hDll.resolve("TSPL_Direction");
    DemFormFeed = (TSPL_FormFeed)m_hDll.resolve("TSPL_FormFeed");
    DemHome = (TSPL_Home)m_hDll.resolve("TSPL_Home");
    DemCodePag  = (TSPL_SetCodePag)m_hDll.resolve("TSPL_SetCodePag");
    DemBitMap = (TSPL_BitMap)m_hDll.resolve("TSPL_BitMap");

    QLibrary zintDll("./zint.dll");
    zintDll.load();
}


