package main

import (
	"fmt"
	"html/template"
	"io"
	"log"
	"net/http"
	"os"
	"os/exec"

	"github.com/i-DarkLight/compiler/src"
)

func main() {
	home := func(w http.ResponseWriter, r *http.Request) {
		templ := template.Must(template.ParseFiles("homepage.html"))
		templ.Execute(w, nil)
	}
	nrparser := func(w http.ResponseWriter, r *http.Request) {
		templ := template.Must(template.ParseFiles("NRParser.html"))
		templ.Execute(w, nil)
		http.Error(w, "Inja Badan vasl mishe :)", http.StatusNoContent)
	}
	playground := func(w http.ResponseWriter, r *http.Request) {
		templ := template.Must(template.ParseFiles("playground.html"))
		templ.Execute(w, nil)
	}
	compile := func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "text/html ;charset=utf-8")
		w.WriteHeader(http.StatusOK)
		if r.Method != "POST" {
			log.Fatal(http.StatusMethodNotAllowed)
		}
		temp := r.PostFormValue("text")
		f, err := os.OpenFile("./data/code.txt", os.O_TRUNC|os.O_WRONLY, 0644)
		if err != nil {
			log.Fatal(err)
		}
		defer r.Body.Close()
		f.WriteString(temp)
		f.Close()
		cmd := exec.Command("./lexical.exe", "quiet", "OPT=XY")
		cmd.Run()
		compiled, _ := os.ReadFile("./data/compiled.txt")
		readerror, _ := os.ReadFile("./data/errors.txt")
		if string(readerror) == "" {
			src.SyntaxChecker(string(compiled))
		}
		readerror, _ = os.ReadFile("./data/errors.txt")
		if string(readerror) != "" {
			io.WriteString(w, string(readerror))
		} else {
			fmt.Fprintf(w, "No Errors Found...\nCompiled Successfully!")
		}
	}
	http.HandleFunc("/", home)
	http.HandleFunc("/nrparser/", nrparser)
	http.HandleFunc("/playground/", playground)
	http.HandleFunc("/playground/compile/", compile)
	http.HandleFunc("/playground/deleteInput/", compile)
	println("Starting server at port 8000...")
	if err := http.ListenAndServe(":8000", nil); err != nil {
		log.Fatal(err)
	}
}
