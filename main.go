package main

import (
	"html/template"
	"io"
	"log"
	"net/http"
	"os"
	"os/exec"
)

func main() {
	h1 := func(w http.ResponseWriter, r *http.Request) {
		templ := template.Must(template.ParseFiles("homepage.html"))
		templ.Execute(w, nil)
	}
	playground:=func(w http.ResponseWriter, r *http.Request){
		templ := template.Must(template.ParseFiles("playground.html"))
		templ.Execute(w,nil)
	}
	h2 := func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "text/html")
		w.WriteHeader(http.StatusOK)
		if r.Method != "POST" {
			log.Fatal(http.StatusMethodNotAllowed)
		}
		temp := r.PostFormValue("text")
		f, err := os.OpenFile(".\\src\\code.txt", os.O_TRUNC|os.O_WRONLY, 0644)
		if err != nil {
			log.Fatal(err)
		}
		defer r.Body.Close()
		f.WriteString(temp)
		f.Close()
		cmd := exec.Command("C:\\Users\\Dark\\compiler\\dlang.exe", "quiet", "OPT=XY")
		cmd.Run()
		readtokens, _ := os.ReadFile(".\\src\\compiled.txt")
		readerror, _ := os.ReadFile(".\\src\\errors.txt")
		if string(readerror) != "" {
			io.WriteString(w, string(readerror))
		} else {
			io.WriteString(w,string(readtokens))
		}
	}
	http.HandleFunc("/", h1)
	http.HandleFunc("/playground/", playground)
	http.HandleFunc("/playground/compile/",h2)
	http.HandleFunc("/playground/deleteInput/", h2)
	println("Starting server at port 8000...")
	if err := http.ListenAndServe(":8000", nil); err != nil {
		log.Fatal(err)
	}
}
