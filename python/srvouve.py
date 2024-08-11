#!/usr/bin/env python3


import speech_recognition as sr
import socket
import threading
from queue import Queue

# Variáveis globais
r = sr.Recognizer()
clients = []  # Lista para manter os clientes conectados

# Fila para o último texto dito
last_said = Queue(maxsize=10)

def broadcast_message(message):
    for client in clients:
        try:
            client.send(message.encode('utf-8'))
        except:
            clients.remove(client)

def client_handler(client_socket):
    while True:
        try:
            message = last_said.get(block=True)  # Bloqueia até que haja algo para enviar
            broadcast_message(message)
        except:
            client_socket.close()
            break

def accept_connections(server):
    while True:
        client_sock, addr = server.accept()
        print(f"Conexão aceita de {addr}")
        clients.append(client_sock)
        client_thread = threading.Thread(target=client_handler, args=(client_sock,))
        client_thread.daemon = True
        client_thread.start()

def setup():
    # Configuração do servidor
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('0.0.0.0', 8097))
    server.listen(5)
    print("Servidor escutando na porta 8097")

    accept_thread = threading.Thread(target=accept_connections, args=(server,))
    accept_thread.daemon = True
    accept_thread.start()

def loop():
    with sr.Microphone() as source:
        # Ajuste para o ruído de fundo fora do loop para não repetir a cada iteração
        print("Ajuste do ruído de fundo. Aguarde...")
        r.adjust_for_ambient_noise(source, duration=1)
        r.pause_threashold = 0.8

        while True:
            # Ajustando o reconhecimento de fala para o ruído ambiente
            print("Ajustando para o ruído ambiente...")
            #r.adjust_for_ambient_noise(source, duration=1)

            # Configurando o pause_threshold (tempo de espera por pausas na fala)
            #r.pause_threshold = 0.8  # 0.8 segundos de pausa serão considerados como uma pausa na fala
 
            print("Fale algo:")
            audio = r.listen(source, phrase_time_limit=5)
            try:
                print("Iniciou analise");
                text = r.recognize_google(audio, language='pt-BR')
                print("Você disse: " + text)
                if last_said.full():  # Se a fila estiver cheia, remove o item mais antigo
                    last_said.get_nowait()
                last_said.put(text)
                broadcast_message(text)
            except sr.UnknownValueError:
                print("Não consegui entender o áudio")
            except sr.RequestError as e:
                print(f"Erro ao solicitar resultados da API; {e}")

if __name__ == "__main__":
    setup()
    loop()

