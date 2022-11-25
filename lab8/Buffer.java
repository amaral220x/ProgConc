import java.util.Random;
class Buffer{
    private int tamanho; 
    private Double buffer[];
    private int entrada, retirada, cont;
    Buffer(int tamanho){
        this.tamanho = tamanho;
        buffer  = new Double[tamanho];
        entrada = 0;
        retirada = 0;
        cont = 0;
    }
    public synchronized void Inserir(Double valor){
        try{
            while(cont==tamanho){
                //Buffer cheio, não é possível inserir elementos.
                System.out.println("Entrada do elemento" + valor + " bloqueada");
                wait();
            }
            //Thread desbloqueada, espaço sobrando.
            buffer[entrada] = valor;
            System.out.println("buffer["+ entrada + "]: " + valor);
            entrada = (entrada + 1) % tamanho;
            cont++;
            System.out.println("Quantidade de elementos = " + cont);
            notifyAll();
        } catch(InterruptedException e){
            System.out.println("ERRORRRR!");
            return;
        }
    }
    
    public synchronized Double Remover(){
        try {
            while(cont == 0){
                //Buffer vazio, nao pode remover.
                System.out.println("Retirada de elemento bloqueada");
                wait();
            }
            //Buffer tem elemento, pode remover.
            Double valor = buffer[retirada];
            System.out.println(valor + "removido de buffer["+ retirada + "]");
            retirada = (retirada + 1) % tamanho;
            cont--;
            System.out.println("Quantidade de elementos = " + cont);
            notifyAll();
            return valor;
        }
        catch(InterruptedException e){
            System.out.println("ERRORRRR!");
            return -1.;
        }
    }
    
    public void Imprimir(){
        for(int i = retirada; i<entrada; i= (i+1)%tamanho){
            System.out.println("buffer[" + i +"] : " + buffer[i]);
        }
    }
}

class Produtora extends Thread{
    private Buffer b;
    private Double valor;
    private int id;
    Random rand;
    Produtora(Buffer b, int id){
        this.b = b;
        this.id = id;
    }
    public void run(){
        rand = new Random();
        try{
            for(;;){
                valor = rand.nextDouble();
                System.out.println("A thread " + id + " vai inserir o elemento " + valor);
                b.Inserir(valor);
                sleep(1000);
            }
        }catch(InterruptedException e){
            System.out.println("ERRORRRR!");
            return; 
        }

    }
}

class Consumidora extends Thread{
    private Buffer b;
    private Double valor;
    private int id;
    Consumidora(Buffer b, int id){
        this.b = b;
        this.id = id;
    }
    public void run(){
        try {
            for(;;){
                System.out.println("A thread "+ id +" vai remover um elemento do buffer");
                valor = b.Remover();
                sleep(1000);
            }
        } catch(InterruptedException e){
            System.out.println("ERRORRRR!");
            return;
        }
    }
}

class Main{
    public static void main(String... args) {
        Buffer b = new Buffer(10);
        int nProdutoras = 10;
        int nConsumidoras = 1;
        int qntdThreads = nProdutoras + nConsumidoras; 
        Thread[] threads = new Thread[qntdThreads];
        
        //Cria threads produtoras e consumidoras.
        for(int i = 0; i<nProdutoras; i++){
            threads[i] = new Produtora(b, i);
        }
        
        for(int i = nProdutoras; i<qntdThreads; i++){
            threads[i] = new Consumidora(b, i);
        }
        //Inicia threads.
        for(int i = 0; i < qntdThreads; i++){
            threads[i].start();
        }
        //Junta as threads.
        for(int i = 0; i < qntdThreads; i++){
            try{
                threads[i].join();
            }catch(InterruptedException e){
                return;
            }
        }        
    }
}