import asyncio  #get signals from camera &send sound to speaker
import websockets   # opem connection between ESP & laptop
import edge_tts   #change sound to voice (by using Microsoft Edge)
from pydub import AudioSegment    # charactristic of sound(as speed)
import io   #store sound in RAM instead of making file


users_database = { #dictionary acts as the robot's "Memory"  search woth id not name 
    "id_101": "Morooj",
    "id_102": ""
    # so onnn
}

VOICE = "en-US-AvaNeural"
connected_clients = set() # how many device connect

async def handle_client(websocket):
    connected_clients.add(websocket)
    print(f"New Device Connected. Total: {len(connected_clients)}")
    try:
        async for message in websocket:
            
            user_id = message.strip()
            
            #  i know this person or not?
            if user_id in users_database:
                name = users_database[user_id]
                text = f"Welcome back, {name}! I remember you.Nice to see you again!"
            else:
                # ask for name if robot donot know
                text = "Hello! I don't think we've met. What is your name?"
                #take name and add
            print(f"Robot says: {text}") # make sure then go to tell name

            # generate voice
            communicate = edge_tts.Communicate(text, VOICE)
            audio_data = b""
            async for chunk in communicate.stream():   # take sound line by line and combine in audio
                if chunk["type"] == "audio":
                    audio_data += chunk["data"]

            # ESP32
            temp_audio = io.BytesIO(audio_data)
            audio = AudioSegment.from_file(temp_audio)
            audio = audio.set_frame_rate(16000).set_channels(1).set_sample_width(1)

           # await websocket.send(audio.raw_data)  not good for ESP

            raw_bytes = audio.raw_data
            chunk_size = 1024 #send 1204 bytes in each time 
            for i in range(0, len(raw_bytes), chunk_size):  #divide sound into small pices
                chunk = raw_bytes[i:i + chunk_size]
                
                await asyncio.gather(
                    *[client.send(chunk) for client in connected_clients]
                )
            print("Audio stream sent.")

    except websockets.exceptions.ConnectionClosed:
        print("Disconnected")
    finally:
        connected_clients.remove(websocket)

async def main():
    host = "0.0.0.0"
    port = 8080      # or 8081
    
    async with websockets.serve(handle_client, host, port):
        print(f"WebSocket Server running on ws://{host}:{port}")
        await asyncio.Future() #run always

asyncio.run(main())