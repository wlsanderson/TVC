import struct
import csv
from os import listdir, getcwd
from os.path import isfile, join 

structsize = 32
chunk_size = 1024
byte_format = "<Lffhhhhhhhhhh"
header = ['time','pressure_pa','temp_c','ID','gyro_x','gyro_y','gyro_z','acc_x','acc_y','acc_z','mag_x','mag_y','mag_z']
gyro_scale_factor = 0.07
acc_scale_factor = 0.000732
mag_scale_factor = 0.014
imu_idx_start = 4

scripts_dir = join(getcwd(), "scripts")
bins = [f for f in listdir(scripts_dir) if f.endswith(".bin")]

bins.reverse()

for file in bins:
    file_name = file[:-4]
    print(file_name)
    with open(join(scripts_dir,bins[0]), "rb") as f, open(join(scripts_dir, file_name + ".csv"), "w", newline='') as out:
        csv_writer = csv.writer(out)
        csv_writer.writerow(header)
        while True:
            error = False
            for i in range(chunk_size // structsize):
                packet_bin = f.read(structsize)
                if not packet_bin or len(packet_bin) != structsize:
                    error = True
                    break
                packet = struct.unpack(byte_format, packet_bin)
                
                if (packet[0] == 0):
                    break
                real_packet = [None] * (imu_idx_start + 9)
                for i in range(imu_idx_start):
                    real_packet[i] = packet[i]
                for i in range(imu_idx_start, imu_idx_start+3):
                    real_packet[i] = float(packet[i]) * gyro_scale_factor
                for i in range(imu_idx_start + 3, imu_idx_start + 6):
                    real_packet[i] = float(packet[i]) * acc_scale_factor
                for i in range(imu_idx_start + 6, imu_idx_start + 9):
                    real_packet[i] = float(packet[i]) * mag_scale_factor
                csv_writer.writerow(real_packet)
                
            skipbytes = chunk_size - (chunk_size // structsize) * structsize
            check = f.read(skipbytes)
            if skipbytes == 0:
                check = 1
            if not check:
                break
            if error:
                break

