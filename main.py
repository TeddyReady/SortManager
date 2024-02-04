import matplotlib.pyplot as plt

if __name__ == "__main__":


    range_x = [100000,10000,5000,2000,1000,500,100]
    plot1 = [3316.47,32.775,8.07,1.292,0.323,0.081,0.003]
    plot2 = [1234.73,9.681,2.424,0.384,0.096,0.024,0.001]
    plot3 = [2.209,0.151,0.071,0.024,0.01,0.004,0.000001]

    plt.plot(range_x, plot1, linewidth=1.0)
    plt.plot(range_x, plot2, linewidth=1.0)
    plt.plot(range_x, plot3, linewidth=1.0)
    plt.show()
