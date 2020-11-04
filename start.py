from tkinter import *
import tkinter as tk
from tkinter import ttk
from tkinter.messagebox import showwarning
from tkinter import messagebox
import sqlite3
from time import strftime
import time
from PIL import Image, ImageTk
import datetime as dt
from tkinter import scrolledtext


# Sprawdzanie czy baza istnieje, jeśli nie to tworzy bazę
conn = sqlite3.connect('baza.db')
c = conn.cursor()
c.execute(
    """CREATE TABLE IF NOT EXISTS user(
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        imie text NOT NULL,
        nazwisko text NOT NULL,
        dzien text NOT NULL,
        miesiąc text NOT NULL,
        rok text NOT NULL,
        ulica text NOT NULL,
        nr_ulicy text NOT NULL,
        nr_mieszkania text NOT NULL,
        miasto text NOT NULL,
        kod text NOT NULL,
        telefon text NOT NULL,
        email text NOT NULL,
        rodzaj_cuk text NOT NULL,
        waga text NOT NULL,
        wzrost text NOT NULL);""")

c.execute(""" CREATE TABLE IF NOT EXISTS doktor(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            imie_dok text NOT NULL,
            nazwisko_dok text NOT NULL,
            specjalizacja_dok text NOT NULL,
            telefon_dok text NOT NULL,
            email_dok text NOT NULL,
            przychodnia_dok text NOT NULL);""")

c.execute(
    """CREATE TABLE IF NOT EXISTS poziomy(
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        dzien text NOT NULL,
        miesiac text NOT NULL,
        rok text NOT NULL,
        godzina text NOT NULL,
        minuta text NOT NULL,
        pora_dnia text NOT NULL,
        wynik text NOT NULL);""")

c.execute(
    """CREATE TABLE IF NOT EXISTS waga(
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        data text NOT NULL,
        godzina text NOT NULL,
        wazenie text NOT NULL);""")
conn.commit()
conn.close()

# *****************************  DEFINICJE  ************************************


def wyniki_pomiarow():
    def zamknij_pomiary():
        ramka.destroy()

    ramka = tk.LabelFrame(root, text="Wyniki pomiarów",
                          bg="#FFD700", padx=5, pady=5)
    ramka.pack(padx=5, pady=5, side=TOP)

    ramka1 = tk.LabelFrame(ramka, bg="#F0E68C", padx=5, pady=5)
    ramka1.pack(fill="both", expand="yes")

    ramka2 = tk.LabelFrame(ramka, bg="#F0E68C", padx=5, pady=5)
    ramka2.pack(fill="both", expand="yes")

    wynik = scrolledtext.ScrolledText(
        ramka1, width=70, height=24, font=("Times New Roman", 13))
    wynik.grid(row=1, column=0)
    conn = sqlite3.connect('baza.db')
    c = conn.cursor()
    c.execute("SELECT * FROM poziomy")
    records = c.fetchall()
    for rec in records:
        wynik.insert(INSERT, str(rec[1])+"/"+str(rec[2])+"/"+str(rec[3])+"  "+str(
            rec[4])+":"+str(rec[5])+"    "+str(rec[6])+"   "+str(rec[7])+"\n")

    conn.close()

    button = Button(ramka2, text="zamknij okno",
                    bg="#FFD700", command=zamknij_pomiary)
    button.pack(side=TOP)


def pomiar_miesiaca():
    def zamknij_okno():
        ramk.destroy()

    def pokaz_pomiary():
        mies = miesiacZap.get()
        if mies == "Styczeń":
            mies = "01"
        elif mies == "Luty":
            mies = "02"
        elif mies == "Marzec":
            mies = "03"
        elif mies == "Kwiecień":
            mies = "04"
        elif mies == "Maj":
            mies = "05"
        elif mies == "Czerwiec":
            mies = "06"
        elif mies == "Lipiec":
            mies == "07"
        elif mies == "Sierpień":
            mies = "08"
        elif mies == "Wrzesień":
            mies = "09"
        elif mies == "Październik":
            mies = "10"
        elif mies == "Listopad":
            mies = "11"
        elif mies == "Grudzień":
            mies = "12"
        else:
            messagebox.showinfo("Informacja", "Brak wyniku")

        wynik = scrolledtext.ScrolledText(ramk2, width=50, height=20)
        wynik.grid(row=1, column=0)
        conn = sqlite3.connect('baza.db')
        c = conn.cursor()
        c.execute("SELECT * FROM poziomy WHERE miesiac = '"+mies+"'")
        records = c.fetchall()
        for rec in records:
            wynik.insert(INSERT, str(rec[1])+"/"+str(rec[2])+"/"+str(rec[3])+"  "+str(
                rec[4])+":"+str(rec[5])+"    "+str(rec[6])+"   "+str(rec[7])+"\n")

        conn.close()

    ramk = tk.LabelFrame(root, text="Pomiary z miesiąca", padx=5, pady=5)
    ramk.pack(padx=5, pady=5, side=TOP)

    ramk1 = tk.LabelFrame(ramk, padx=5, pady=5)
    ramk1.pack(fill="both", expand="yes")

    ramk2 = tk.LabelFrame(ramk, padx=5, pady=5)
    ramk2.pack(fill="both", expand="yes")

    miesiacZap = StringVar()

    zap = ttk.Combobox(ramk1, textvariable=miesiacZap,
                       font=("Arial", 13), width=12)
    zap['values'] = ("Styczeń", "Luty", "Marzec", "Kwiecień", "Maj", "Czerwiec",
                     "Lipiec", "Sierpień", "Wrzesień", "Pażdziernik", "Listopad", "Grudzień")
    zap.current(0)
    zap.grid(row=0, column=0)

    button = tk.Button(ramk1, text="Pokaż pomiary", command=pokaz_pomiary)
    button1 = tk.Button(ramk1, text="Zamknij okno", command=zamknij_okno)
    button.grid(row=0, column=1)
    button1.grid(row=0, column=2)


def rejestracja_lekarza():

    def formularz():

        def zapisz_dok():
            INP = ent5.get("1.0", "end-1c")
            conn = sqlite3.connect('baza.db')
            c = conn.cursor()
            c.execute("INSERT INTO doktor VALUES(NULL, :imie_dok, :nazwisko_dok, :specjalizacja_dok, :telefon_dok, :email_dok, :przychodnia_dok)",
                      {
                          'imie_dok': imieDok.get(),
                          'nazwisko_dok': nazwiskoDok.get(),
                          'specjalizacja_dok': specjalizacjaDok.get(),
                          'telefon_dok': telefonDok.get(),
                          'email_dok': emailDok.get(),
                          'przychodnia_dok': INP

                      })
            conn.commit()
            conn.close()
            messagebox.showinfo("Informacja", "Nowy lekarz zapisany!!!")
            ram.destroy()

        def zamknij_dok():
            ram.destroy()

        def wyczysc_dok():
            ent.delete(0, END)
            ent1.delete(0, END)
            ent2.delete(0, END)
            ent3.delete(0, END)
            ent4.delete(0, END)

        ram = tk.LabelFrame(root, text="Nowy lekarz", padx=5, pady=5)
        ram.pack(padx=5, pady=5, side=TOP)

        ram1 = tk.LabelFrame(ram, padx=5, pady=5)
        ram1.pack(fill="both", expand="yes")

        ram2 = tk.LabelFrame(ram, padx=5, pady=5)
        ram2.pack(fill="both", expand="yes")

        ram3 = tk.LabelFrame(ram, padx=5, pady=5)
        ram3.pack(fill="both", expand="yes")

        ram4 = tk.LabelFrame(ram, padx=5, pady=5)
        ram4.pack(fill="both", expand="yes")

        lin1 = tk.Label(ram1, text="Imie lekarza: ",
                        font=("Arial", 13), fg="blue")
        lin2 = tk.Label(ram1, text="Nazwisko lekarza: ",
                        font=("Arial", 13), fg="blue")
        lin3 = tk.Label(ram1, text="Specjalizacja: ",
                        font=("Arial", 13), fg="blue")

        lin1.grid(row=0, column=0, sticky=W)
        lin2.grid(row=1, column=0, sticky=W)
        lin3.grid(row=2, column=0, sticky=W)

        lin4 = tk.Label(ram2, text="telefon: ", font=("Arial", 13), fg="blue")
        lin5 = tk.Label(ram2, text="email: ", font=("Arial", 13), fg="blue")

        lin4.grid(row=0, column=0, sticky=W)
        lin5.grid(row=1, column=0, sticky=W)

        lin6 = tk.Label(ram3, text="Przychodnia: ",
                        font=("Arial", 13), fg="blue")
        lin6.grid(row=0, column=0, sticky=W)

        imieDok = StringVar()
        nazwiskoDok = StringVar()
        specjalizacjaDok = StringVar()
        telefonDok = StringVar()
        emailDok = StringVar()

        ent = tk.Entry(ram1, textvariable=imieDok, width=32)
        ent1 = tk.Entry(ram1, textvariable=nazwiskoDok, width=32)
        ent2 = tk.Entry(ram1, textvariable=specjalizacjaDok, width=32)

        ent.grid(row=0, column=1)
        ent1.grid(row=1, column=1)
        ent2.grid(row=2, column=1)

        ent3 = tk.Entry(ram2, textvariable=telefonDok, width=25)
        ent4 = tk.Entry(ram2, textvariable=emailDok, width=25)

        ent3.grid(row=0, column=1)
        ent4.grid(row=1, column=1)

        ent5 = Text(ram3, width=30, height=5, font=("Arial", 13), wrap="word")
        ent5.grid(row=0, column=1)

        but1 = tk.Button(ram4, text="Zapisz", fg="red", command=zapisz_dok)
        but2 = tk.Button(ram4, text="Wyczyść", fg="blue", command=wyczysc_dok)
        but3 = tk.Button(ram4, text="Zamknij", fg="blue", command=zamknij_dok)

        but1.grid(row=0, column=0)
        but2.grid(row=0, column=1)
        but3.grid(row=0, column=2)

    conn = sqlite3.connect('baza.db')
    c = conn.cursor()
    c.execute("SELECT COUNT(*) FROM doktor")
    records = c.fetchone()
    row = records[0]
    if row > 0:
        result = messagebox.askquestion(
            "Pytanie?", "Lekarz już istnieje, czy chcesz wprowadzić nowego użytkownika?", icon="warning")
        if result == "yes":
            formularz()
        else:
            return
    else:
        formularz()


def rejestracja_uzytkownika():

    def zamknij_usera():
        ramkaNowyUser.destroy()

    def wyczysc_usera():
        en1.delete(0, END)
        en2.delete(0, END)
        en6.delete(0, END)
        en7.delete(0, END)
        en8.delete(0, END)
        en9.delete(0, END)
        en10.delete(0, END)
        en11.delete(0, END)

    def zapisz_usera():
        conn = sqlite3.connect('baza.db')
        c = conn.cursor()
        c.execute("INSERT INTO user VALUES(NULL, :imie, :nazwisko, :dzien, :miesiac, :rok, :ulica, :nr_ulicy, :nr_mieszkania, :miasto, :kod, :telefon, :email, :rodzaj_cuk, :waga, :wzrost)",
                  {
                      'imie': imie_ins.get(),
                      'nazwisko': nazwisko_ins.get(),
                      'dzien': dzien_ins.get(),
                      'miesiac': miesiac_ins.get(),
                      'rok': rok_ins.get(),
                      'ulica': ulica_ins.get(),
                      'nr_ulicy': nrUlicy_ins.get(),
                      'nr_mieszkania': nrMieszkania_ins.get(),
                      'miasto': miasto_ins.get(),
                      'kod': kod_ins.get(),
                      'telefon': telefon_ins.get(),
                      'email': email_ins.get(),
                      'rodzaj_cuk': rodzCukrzycy_ins.get(),
                      'waga': waga_ins.get(),
                      'wzrost': wzrost_ins.get()
                  })
        conn.commit()
        conn.close()
        messagebox.showinfo("Informacja", "Nowy użytkownik zapisany!!!")
        ramkaNowyUser.destroy()

    conn = sqlite3.connect('baza.db')
    c = conn.cursor()
    c.execute("SELECT COUNT(*) FROM user")
    records = c.fetchone()
    row = records[0]
    if row > 0:
        result = messagebox.askquestion(
            "Pytanie", "Użytkownik już istnieje, czy chcesz wprowadzić nowego użytkownika?", icon="warning")
        if result == "yes":

            ramkaNowyUser = LabelFrame(
                root, text="Nowy użytkownik", padx=5, pady=5)
            ramkaNowyUser.pack(padx=5, pady=5, side=TOP)

            ramkaNowyUser_1 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
            ramkaNowyUser_1.pack(fill="both", expand="yes")

            ramkaNowyUser_2 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
            ramkaNowyUser_2.pack(fill="both", expand="yes")

            ramkaNowyUser_3 = LabelFrame(
                ramkaNowyUser, text="Adres zamieszkania", padx=5, pady=5)
            ramkaNowyUser_3.pack(fill="both", expand="yes")

            ramkaNowyUser_4 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
            ramkaNowyUser_4.pack(fill="both", expand="yes")

            ramkaNowyUser_5 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
            ramkaNowyUser_5.pack(fill="both", expand="yes")

            ramkaNowyUser_6 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
            ramkaNowyUser_6.pack(fill="both", expand="yes")

            li1 = tk.Label(ramkaNowyUser_1, text="Imie: ",
                           font=("Arial", 13), fg="green")
            li2 = tk.Label(ramkaNowyUser_1, text="Nazwisko: ",
                           font=("Arial", 13), fg="green")
            li1.grid(row=0, column=0, sticky=W)
            li2.grid(row=1, column=0, sticky=W)

            li3 = tk.Label(ramkaNowyUser_2, text="Urodzony: ",
                           font=("Arial", 13), fg="green")
            li3.grid(row=0, column=0, sticky=W)

            li5 = tk.Label(ramkaNowyUser_3, text="ulica: ",
                           font=("Arial", 13), fg="green")
            li6 = tk.Label(ramkaNowyUser_3, text="nr ulicy: ",
                           font=("Arial", 13), fg="green")
            li7 = tk.Label(ramkaNowyUser_3, text="nr mieszkania: ",
                           font=("Arial", 13), fg="green")
            li8 = tk.Label(ramkaNowyUser_3, text="kod: ",
                           font=("Arial", 13), fg="green")
            li9 = tk.Label(ramkaNowyUser_3, text="miasto: ",
                           font=("Arial", 13), fg="green")
            li5.grid(row=0, column=0, sticky=W)
            li6.grid(row=1, column=0, sticky=W)
            li7.grid(row=2, column=0, sticky=W)
            li8.grid(row=3, column=0, sticky=W)
            li9.grid(row=4, column=0, sticky=W)

            li10 = tk.Label(ramkaNowyUser_4, text="Telefon: ",
                            font=("Arial", 13), fg="green")
            li11 = tk.Label(ramkaNowyUser_4, text="Email: ",
                            font=("Arial", 13), fg="green")
            li10.grid(row=0, column=0, sticky=W)
            li11.grid(row=1, column=0, sticky=W)

            li12 = tk.Label(ramkaNowyUser_5, text="Rodzaj cukrzycy: ", font=(
                "Arial", 13), fg="green")
            li13 = tk.Label(ramkaNowyUser_5, text="Waga: ",
                            font=("Arial", 13), fg="green")
            li14 = tk.Label(ramkaNowyUser_5, text="Wzrost: ",
                            font=("Arial", 13), fg="green")
            li12.grid(row=0, column=0, sticky=W)
            li13.grid(row=1, column=0, sticky=W)
            li14.grid(row=2, column=0, sticky=W)

            li20 = tk.Label(ramkaNowyUser_5, text="kg",
                            font=("Arial", 13), fg="green")
            li21 = tk.Label(ramkaNowyUser_5, text="cm",
                            font=("Arial", 13), fg="green")
            li20.grid(row=1, column=3, sticky=W)
            li21.grid(row=2, column=3, sticky=W)

            imie_ins = StringVar()
            nazwisko_ins = StringVar()
            dzien_ins = StringVar()
            miesiac_ins = StringVar()
            rok_ins = StringVar()
            ulica_ins = StringVar()
            nrUlicy_ins = StringVar()
            nrMieszkania_ins = StringVar()
            kod_ins = StringVar()
            miasto_ins = StringVar()
            telefon_ins = StringVar()
            email_ins = StringVar()
            rodzCukrzycy_ins = StringVar()
            waga_ins = StringVar()
            wzrost_ins = StringVar()

            en1 = tk.Entry(ramkaNowyUser_1, textvariable=imie_ins, width=31)
            en2 = tk.Entry(ramkaNowyUser_1,
                           textvariable=nazwisko_ins, width=31)
            en1.grid(row=0, column=1)
            en2.grid(row=1, column=1)

            en3 = tk.Spinbox(ramkaNowyUser_2, from_=1, to=31,
                             textvariable=dzien_ins, font=("Arial", 13), width=4)
            en4 = ttk.Combobox(
                ramkaNowyUser_2, textvariable=miesiac_ins, font=("Arial", 13), width=12)
            en4['values'] = ("Styczeń", "Luty", "Marzec", "Kwiecień", "Maj", "Czerwiec",
                             "Lipiec", "Sierpień", "Wrzesień", "Pażdziernik", "Listopad", "Grudzień")
            en4.current(1)
            en5 = tk.Spinbox(ramkaNowyUser_2, from_=1960, to=2020,
                             textvariable=rok_ins, font=("Arial", 13), width=4)
            en3.grid(row=0, column=1)
            en4.grid(row=0, column=2)
            en5.grid(row=0, column=3)

            en6 = Entry(ramkaNowyUser_3, textvariable=ulica_ins, width=27)
            en7 = Entry(ramkaNowyUser_3, textvariable=nrUlicy_ins, width=10)
            en8 = Entry(ramkaNowyUser_3,
                        textvariable=nrMieszkania_ins, width=10)
            en9 = Entry(ramkaNowyUser_3, textvariable=kod_ins, width=10)
            en10 = Entry(ramkaNowyUser_3, textvariable=miasto_ins, width=27)
            en6.grid(row=0, column=1)
            en7.grid(row=1, column=1, sticky=W)
            en8.grid(row=2, column=1, sticky=W)
            en9.grid(row=3, column=1, sticky=W)
            en10.grid(row=4, column=1)

            en11 = Entry(ramkaNowyUser_4, textvariable=telefon_ins, width=33)
            en12 = Entry(ramkaNowyUser_4, textvariable=email_ins, width=33)
            en11.grid(row=0, column=1)
            en12.grid(row=1, column=1)

            en13 = tk.Spinbox(ramkaNowyUser_5, from_=1, to=2,
                              textvariable=rodzCukrzycy_ins, font=("Arial", 13), width=2)
            en14 = tk.Spinbox(ramkaNowyUser_5, from_=1, to=180,
                              textvariable=waga_ins, font=("Arial", 13), width=4)
            en15 = tk.Spinbox(ramkaNowyUser_5, from_=1, to=230, font=(
                "Arial", 13), textvariable=wzrost_ins, width=4)
            en13.grid(row=0, column=1, sticky=W)
            en14.grid(row=1, column=1, sticky=W)
            en15.grid(row=2, column=1, sticky=W)

            bu1 = tk.Button(ramkaNowyUser_6, text="Zapisz dane",
                            command=zapisz_usera)
            bu2 = tk.Button(ramkaNowyUser_6, text="Wyczyść pola",
                            command=wyczysc_usera)
            bu3 = tk.Button(ramkaNowyUser_6, text="Zamknij okno",
                            command=zamknij_usera)
            bu1.grid(row=0, column=0)
            bu2.grid(row=0, column=1)
            bu3.grid(row=0, column=2)

        else:
            return

    else:
        ramkaNowyUser = LabelFrame(
            root, text="Nowy użytkownik", padx=5, pady=5)
        ramkaNowyUser.pack(padx=5, pady=5, side=TOP)

        ramkaNowyUser_1 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
        ramkaNowyUser_1.pack(fill="both", expand="yes")

        ramkaNowyUser_2 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
        ramkaNowyUser_2.pack(fill="both", expand="yes")

        ramkaNowyUser_3 = LabelFrame(
            ramkaNowyUser, text="Adres zamieszkania", padx=5, pady=5)
        ramkaNowyUser_3.pack(fill="both", expand="yes")

        ramkaNowyUser_4 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
        ramkaNowyUser_4.pack(fill="both", expand="yes")

        ramkaNowyUser_5 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
        ramkaNowyUser_5.pack(fill="both", expand="yes")

        ramkaNowyUser_6 = LabelFrame(ramkaNowyUser, padx=5, pady=5)
        ramkaNowyUser_6.pack(fill="both", expand="yes")

        li1 = tk.Label(ramkaNowyUser_1, text="Imie: ",
                       font=("Arial", 13), fg="green")
        li2 = tk.Label(ramkaNowyUser_1, text="Nazwisko: ",
                       font=("Arial", 13), fg="green")
        li1.grid(row=0, column=0, sticky=W)
        li2.grid(row=1, column=0, sticky=W)
        li3 = tk.Label(ramkaNowyUser_2, text="Urodzony: ",
                       font=("Arial", 13), fg="green")
        li3.grid(row=0, column=0, sticky=W)

        li5 = tk.Label(ramkaNowyUser_3, text="ulica: ",
                       font=("Arial", 13), fg="green")
        li6 = tk.Label(ramkaNowyUser_3, text="nr ulicy: ",
                       font=("Arial", 13), fg="green")
        li7 = tk.Label(ramkaNowyUser_3, text="nr mieszkania: ",
                       font=("Arial", 13), fg="green")
        li8 = tk.Label(ramkaNowyUser_3, text="kod: ",
                       font=("Arial", 13), fg="green")
        li9 = tk.Label(ramkaNowyUser_3, text="miasto: ",
                       font=("Arial", 13), fg="green")
        li5.grid(row=0, column=0, sticky=W)
        li6.grid(row=1, column=0, sticky=W)
        li7.grid(row=2, column=0, sticky=W)
        li8.grid(row=3, column=0, sticky=W)
        li9.grid(row=4, column=0, sticky=W)

        li10 = tk.Label(ramkaNowyUser_4, text="Telefon: ",
                        font=("Arial", 13), fg="green")
        li11 = tk.Label(ramkaNowyUser_4, text="Email: ",
                        font=("Arial", 13), fg="green")
        li10.grid(row=0, column=0, sticky=W)
        li11.grid(row=1, column=0, sticky=W)

        li12 = tk.Label(ramkaNowyUser_5, text="Rodzaj cukrzycy: ",
                        font=("Arial", 13), fg="green")
        li13 = tk.Label(ramkaNowyUser_5, text="Waga: ",
                        font=("Arial", 13), fg="green")
        li14 = tk.Label(ramkaNowyUser_5, text="Wzrost: ",
                        font=("Arial", 13), fg="green")
        li12.grid(row=0, column=0, sticky=W)
        li13.grid(row=1, column=0, sticky=W)
        li14.grid(row=2, column=0, sticky=W)

        li20 = tk.Label(ramkaNowyUser_5, text="kg",
                        font=("Arial", 13), fg="green")
        li21 = tk.Label(ramkaNowyUser_5, text="cm",
                        font=("Arial", 13), fg="green")
        li20.grid(row=1, column=3, sticky=W)
        li21.grid(row=2, column=3, sticky=W)

        imie_ins = StringVar()
        nazwisko_ins = StringVar()
        dzien_ins = StringVar()
        miesiac_ins = StringVar()
        rok_ins = StringVar()
        ulica_ins = StringVar()
        nrUlicy_ins = StringVar()
        nrMieszkania_ins = StringVar()
        kod_ins = StringVar()
        miasto_ins = StringVar()
        telefon_ins = StringVar()
        email_ins = StringVar()
        rodzCukrzycy_ins = StringVar()
        waga_ins = StringVar()
        wzrost_ins = StringVar()

        en1 = tk.Entry(ramkaNowyUser_1, textvariable=imie_ins, width=31)
        en2 = tk.Entry(ramkaNowyUser_1, textvariable=nazwisko_ins, width=31)
        en1.grid(row=0, column=1)
        en2.grid(row=1, column=1)

        en3 = tk.Spinbox(ramkaNowyUser_2, from_=1, to=31,
                         textvariable=dzien_ins, font=("Arial", 13), width=4)
        en4 = ttk.Combobox(
            ramkaNowyUser_2, textvariable=miesiac_ins, font=("Arial", 13), width=12)
        en4['values'] = ("Styczeń", "Luty", "Marzec", "Kwiecień", "Maj", "Czerwiec",
                         "Lipiec", "Sierpień", "Wrzesień", "Pażdziernik", "Listopad", "Grudzień")
        en4.current(1)
        en5 = tk.Spinbox(ramkaNowyUser_2, from_=1960, to=2020,
                         textvariable=rok_ins, font=("Arial", 13), width=4)
        en3.grid(row=0, column=1)
        en4.grid(row=0, column=2)
        en5.grid(row=0, column=3)

        en6 = Entry(ramkaNowyUser_3, textvariable=ulica_ins, width=27)
        en7 = Entry(ramkaNowyUser_3, textvariable=nrUlicy_ins, width=10)
        en8 = Entry(ramkaNowyUser_3, textvariable=nrMieszkania_ins, width=10)
        en9 = Entry(ramkaNowyUser_3, textvariable=kod_ins, width=10)
        en10 = Entry(ramkaNowyUser_3, textvariable=miasto_ins, width=27)
        en6.grid(row=0, column=1)
        en7.grid(row=1, column=1, sticky=W)
        en8.grid(row=2, column=1, sticky=W)
        en9.grid(row=3, column=1, sticky=W)
        en10.grid(row=4, column=1)

        en11 = Entry(ramkaNowyUser_4, textvariable=telefon_ins, width=33)
        en12 = Entry(ramkaNowyUser_4, textvariable=email_ins, width=33)
        en11.grid(row=0, column=1)
        en12.grid(row=1, column=1)

        en13 = tk.Spinbox(ramkaNowyUser_5, from_=1, to=2,
                          textvariable=rodzCukrzycy_ins, font=("Arial", 13), width=2)
        en14 = tk.Spinbox(ramkaNowyUser_5, from_=1, to=180,
                          textvariable=waga_ins, font=("Arial", 13), width=4)
        en15 = tk.Spinbox(ramkaNowyUser_5, from_=1, to=230, font=(
            "Arial", 13), textvariable=wzrost_ins, width=4)
        en13.grid(row=0, column=1, sticky=W)
        en14.grid(row=1, column=1, sticky=W)
        en15.grid(row=2, column=1, sticky=W)

        bu1 = tk.Button(ramkaNowyUser_6, text="Zapisz dane",
                        command=zapisz_usera)
        bu2 = tk.Button(ramkaNowyUser_6, text="Wyczyść pola",
                        command=wyczysc_usera)
        bu3 = tk.Button(ramkaNowyUser_6, text="Zamknij okno",
                        command=zamknij_usera)
        bu1.grid(row=0, column=0)
        bu2.grid(row=0, column=1)
        bu3.grid(row=0, column=2)


def add_poziomy():
    ramka_poziomy = LabelFrame(root, text="Poziomy", padx=5, pady=5)
    ramka_poziomy.pack(padx=5, pady=5, side=TOP)

    def zapisz_pomiar(d, m, r, g, min):
        poraDniaZapis = poraDnia.get()
        pomiarZapis = pomiar.get()

        conn = sqlite3.connect('baza.db')
        c = conn.cursor()
        c.execute("INSERT INTO poziomy VALUES(NULL, :dzien, :miesiac, :rok, :godzina, :minuta, :pora_dnia, :wynik)",
                  {
                      'dzien': d,
                      'miesiac': m,
                      'rok': r,
                      'godzina': g,
                      'minuta': min,
                      'pora_dnia': poraDniaZapis,
                      'wynik': pomiarZapis
                  })
        conn.commit()
        conn.close()
        messagebox.showinfo("Informacja", "Nowy pomiar zapisany!!!")

    dzien = strftime('%e')
    miesiac = strftime('%m')
    rok = strftime('%Y')
    godzina = strftime('%H')
    minuta = strftime('%M')

    l1 = tk.Label(ramka_poziomy, text="data: ",
                  font=("Arial", 13, "bold"), fg="green")
    l2 = tk.Label(ramka_poziomy, text="godzina: ",
                  font=("Arial", 13, "bold"), fg="green")
    l3 = tk.Label(ramka_poziomy, text="pora dnia: ",
                  font=("Arial", 13, "bold"), fg="green")
    l4 = tk.Label(ramka_poziomy, text="wpisz poziom: ",
                  font=("Arial", 13, "bold"), fg="green")
    l1.grid(row=0, column=0, sticky=W)
    l2.grid(row=0, column=2, sticky=W)
    l3.grid(row=0, column=4, sticky=W)
    l4.grid(row=0, column=6, sticky=W)

    l5 = tk.Label(ramka_poziomy, text=dzien+"-"+miesiac +
                  "-"+rok, font=("arial", 13), fg="green")
    l6 = tk.Label(ramka_poziomy, text=godzina+":" +
                  minuta, font=("arial", 13), fg="green")
    l5.grid(row=0, column=1, sticky=W)
    l6.grid(row=0, column=3, sticky=W)

    poraDnia = StringVar()
    suwak = ttk.Combobox(ramka_poziomy, textvariable=poraDnia)
    suwak['values'] = ("Przed śniadaniem", "Po śniadaniu", "Przed obiadem",
                       "Po obiedzie", "Przed kolacją", "Po kolacji", "Pomiar dodatkowy")
    suwak.current(0)
    suwak.grid(column=5, row=0, sticky=W)

    pomiar = StringVar()
    e1 = Entry(ramka_poziomy, textvariable=pomiar, width=5)
    e1.grid(row=0, column=7)

    l7 = tk.Label(ramka_poziomy, text=" ")
    l7.grid(row=1, column=0)

    b1 = tk.Button(ramka_poziomy, text="Zapisz wynik", fg="red",
                   command=lambda: zapisz_pomiar(dzien, miesiac, rok, godzina, minuta))
    b2 = tk.Button(ramka_poziomy, text="Zamknij to okno",
                   command=ramka_poziomy.destroy)
    b1.grid(row=2, column=4)
    b2.grid(row=2, column=5)


def MenuRozwijane():
    menu = Menu(root)

    new_item = Menu(menu)
    menu.add_cascade(label="Użytkownik", menu=new_item)
    new_item.add_command(label="rejestracja użytkownika",
                         command=rejestracja_uzytkownika)
    new_item.add_separator()
    new_item.add_command(label="rejestracja lekarza",
                         command=rejestracja_lekarza)

    new_obsl = Menu(menu)
    menu.add_cascade(label="Cukier", menu=new_obsl)
    new_obsl.add_command(label="dodaj pomiar", command=add_poziomy)
    new_obsl.add_separator()
    new_obsl.add_command(label="sprawdź pomiar z miesiąca",
                         command=pomiar_miesiaca)
    new_obsl.add_separator()
    new_obsl.add_command(label="sprawdź wszystkie pomiary",
                         command=wyniki_pomiarow)
    new_obsl.add_separator()
    new_obsl.add_command(label="statystyka graficzna", command="")

    root.config(menu=menu)

# **************************  PROGRAM  *****************************


# Tworzenie okna
root = tk.Tk()
root.geometry("800x600+100+100")
root.title("Moja Cukrzyca")

# Blokada geometrii okna
root.resizable(False, False)

# Likwidacja paska stanu
# root.overrideredirect(True)

MenuRozwijane()

root.mainloop()
