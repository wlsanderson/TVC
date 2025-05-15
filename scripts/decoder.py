import struct
import csv
from os import listdir, getcwd
from os.path import isfile, join 

structsize = 48
chunk_size = 1024
byte_format = "<Lfffffffffff"
header = ['time','pressure_pa','temp_c','gyro_x','gyro_y','gyro_z','acc_x','acc_y','acc_z','mag_x','mag_y','mag_z']


scripts_dir = join(getcwd(), "scripts")
bins = [f for f in listdir(scripts_dir) if f.endswith(".bin")]

# with open(join(scripts_dir,bins[0]), 'rb') as file:
#     packet = file.read(structsize)
#     packet_dec = struct.unpack(byte_format, packet)
#     print(packet_dec)
#     packet = file.read(structsize)
#     packet_dec = struct.unpack(byte_format, packet)
#     print(packet_dec)

bins.reverse()

for file in bins:
    file_name = file[:-4]
    print(file_name)
    with open(join(scripts_dir,bins[0]), "rb") as f, open(join(scripts_dir, file_name + ".csv"), "w", newline='') as out:
        csv_writer = csv.writer(out)
        csv_writer.writerow(header)
        while True:
            for i in range(chunk_size // structsize):
                packet_bin = f.read(structsize)
                if not packet_bin or len(packet_bin) != structsize:
                    break
                packet = struct.unpack(byte_format, packet_bin)
                if (packet[0] == 0):
                    break
                #print(packet)
                csv_writer.writerow(packet)
                
            skipbytes = chunk_size - (chunk_size // structsize) * structsize
            check = f.read(skipbytes)
            if not check:
                break

