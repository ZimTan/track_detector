import glob
import numpy as np
import matplotlib.pyplot as plt
import sys

if (len(sys.argv) == 2):

    dataset_path = str(sys.argv[1])
    dataset_res_path = dataset_path + "frag/"

    img = glob.glob(dataset_path + "*.npy")
    img_res = glob.glob(dataset_res_path + "*.npy")

    for i in range(len(img_res)):
        fig, axs = plt.subplots(ncols=2)
        axs[0].imshow(np.load(img[i]))
        axs[1].imshow(np.load(img_res[i]))
        plt.show()

else:
    print("Error : Wrong number of  arguments : Expected 2")
