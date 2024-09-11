# -*- coding: utf-8 -*-
"""
Created on Tue Jul 12 06:52:21 2022

@author: Muriel
"""
from PyQt5 import  *
from serial import  *
from
from PyQt5.QtWidgets import *
from PyQt5.uic import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from math import *
from random import *
from sqlite3 import *
from serial import *
from datetime import *
from time import *

import sys
now = datetime.now()
NotreTime = now.strftime("%H : %M : %S")

max = 100
min = 0

def afficherCode(valeur):
    try:
        if(int(valeur)>=0 or int(valeur) <=9):
            windows.sortieCode.setText(valeur)
        elif (len(valeur) == 6):
            EEPROM = valeur
        else:
            traitement = data.plit(":")
            
            try:
                gauche = traitement[0]
                droite = int(traitement[1])
                if(traintement != "" ):
                    traitement[1] = str(traitement[1])
                    
                    windows.Res.setText(traitement[0])
                    windows.lcdNumber.setText(traitement[1])
                    
            except:
                print("ERREUR D'AFFICHAGE sur LCD!")
    except:
        print("ERREUR D'AFFICHAGE !")

def seting():
    winPara.move(randrange(min, max), randrange(min,max))
    winPara.show()

def signup():
    winLogin.move(randrange(min, max), randrange(min,max))
    winLogin.show()

def connexionDB():
    try:
        connexion = connect("base.db")
        curseur = connexion.cursor()
        curseur.execute("""CREATE TABLE "famille" (
	"id"	INTEGER UNIQUE,
	"name"	TEXT NOT NULL,
	"firstname"	TEXT NOT NULL UNIQUE,
	"number"	TEXT NOT NULL UNIQUE,
	"code"	TEXT NOT NULL,
	"password"	TEXT NOT NULL UNIQUE,
	PRIMARY KEY("id" AUTOINCREMENT)
);""")
        try:
            curseur.execute("SELECT password FROM famille;")
            donner = curseur.fetchall()
            
            Edonner = ""
            
            for donner in donner:
                Edonner += str(donner) + "\n"
                
                time.sleep(1)
                dbcode = Edonner.split()
                
                arduino.write(dbcode.encode())
                if (data == "E"):
                   i = 0
                arduino.close()
                
        except:
            pass
        
        connexion.commit()
    except:
        print("Connexion échouer !")
    finally:
        connexion.close()

def recupererDonner():
    name = winLogin.nom.text()
    lastname = winLogin.prenom.text()
    number = winLogin.numero.text()
    code = winLogin.code.text() 
    password = winLogin.motDePasse.text()
    
    envoyerDB(name, lastname, number, code, password)
    


def envoyerDB(name, lastname, number, code, password):
    try:
        connexionDB()
        connexion = connect("base.db")
        curseur = connexion.cursor()
        curseur.execute("INSTER INTO famille(name, lastname, number, code, password ) VALUES ('{}','{}','{}','{}','{}')".format(name, lastname, number, code, password))
        
    except:
        println("Insertion Echouer")

if __name__ == '__main__':
    App = QApplication(sys.argv)
    windows = loadUi("UI/interface.ui")
    windows.show()
    winPara = loadUi("UI/configuration.ui")
    winLogin = loadUi("UI/enregistrement.ui")
    
    #Signal pour le parametre:
    windows.para.clicked.connect(seting)
    
    #Signal du login:
    windows.add.clicked.connect(signup)
    
    #Signal pour envoyer dans la base de donner
    winLogin.Valider.clicked.connect(recupererDonner)
    
    arduino = Serial(port='COM1', baudrate= 19200)

    i = 1

    while (i>0):
        data = arduino.readline()
        time.sleep(1)
        afficherCode(data)
        
    connexionDB()
    
        
    sys.exit(App.exec_())
    