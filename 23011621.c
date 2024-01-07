#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct{
    double coefficient;
    double exponent;
}X; //her bir polinomal X degerini temsil ediyor mesela 3x^5 (coefficient = 3 , exponent = 5.)Sabit sayi olarak mesela 5 icin coefficient 5 exponent 0 verilecek(5x^0);
typedef struct{
    int trigo_type; //trigonometrinin tipini (sin,cos,tan,cot) ayirt ediyor.
    X x;
    double coefficient;
}TRIGONOMETRICS; //her bir trigonometrik X degerini temsil ediyor mesela 5sin(2x^3);

typedef struct{
    int label; //eger label 0 ise polynominal element kullanilacak, eger label 1 ise trigonometrik;
    X polynominal_elements;
    TRIGONOMETRICS trigonometric_elements;
   
}F_X;

typedef struct{
    double coeff_A;
    double coeff_B;
    F_X *f_x_function;
    int num_elements;
}DIFFERANTIAL_EQUATION; //Diferansiyel denklemi temsil ediyor: Ay' + By = f(x), f(x) fonksiyonunu ve A, B katsayilarini kullanıcı verecek;

double runge_kutta(DIFFERANTIAL_EQUATION dif_EQ, double xi, double yi, double h, double x_reach); //runge kutta yapacak recursive bir metod;

double f_xi_yi(DIFFERANTIAL_EQUATION dif_EQ,double xi, double yi,double h); //f(xi,yi) degerini hesaplicak fonksiyon. f(x,y) dedigimiz sey Ay' + By = f(x) diferansiyel denkleminde y' ifadesinin yalniz birakilmis hali: y' = (f(x) - By) / A; runge kutta icerisinde kullanilacak;

void print_equation(DIFFERANTIAL_EQUATION dif_EQ); // denklemimizi yazdirmaya yarayacak fonksiyon mesela y' + 3y = 13sin(2x) yazdiracak; (katsayilar double oldugu .00 li yazacak);

int main(){
    double xi,yi,h,x_next,estimated_value,real_value;
    DIFFERANTIAL_EQUATION df_eq;
    df_eq.num_elements = 0;
    df_eq.f_x_function = (F_X *)malloc(sizeof(F_X));
    if(df_eq.f_x_function == NULL){
        printf("Memory allocation failed.\n");
        return 1;
    }
    int total_elements,i,choose;
    printf("\n\nWelcome to the program for solving numerical differantial equations using the Runge-Kutta 4 method.\n");
    printf("****************************************************************************************************\n");
    printf("The equation should be entered in the following format: Ay' + By = f(x) and f(x) is a polynomial.\n");
    printf("Enter the coefficient of A | (Ay' + By = f(x)): ");
    scanf("%lf",&df_eq.coeff_A);
    printf("Enter the coefficient of B | (Ay' + By = f(x)): ");
    scanf("%lf",&df_eq.coeff_B);
    printf("How many elements do you want to add for f(x)? | (Ay' + By = f(x)): ");
    scanf("%d",&total_elements);
    for(i = 0;i<total_elements;i++){
        printf("What is your elements' type?\n");
        int type;
        do{
            printf("enter 1 to add polynominal element or enter 2 to add trigonometric element(e.g. polynominal mean x^2 , e.g. trigonometric mean cos(x)): ");
            scanf("%d",&type);
        }while(type != 1 && type !=2); //kullanici 1 secerse polinom tarzi bir eleman girecek 2 secerse trigonometric eleman girecek.

        if(type == 1){
            df_eq.f_x_function = (F_X *)realloc(df_eq.f_x_function,(df_eq.num_elements+1)*sizeof(F_X));
            if(df_eq.f_x_function == NULL){
                printf("Memory allocation failed.\n");
                return 1;
            }
            df_eq.f_x_function[df_eq.num_elements].label = 0; //bu elemanin polinom oldugunu belirtmek amacli bir label.
            printf("Enter the coefficient for %d. element (If you want to enter a constant number, set the exponent to 0.): ",i+1);
            scanf("%lf",&df_eq.f_x_function[df_eq.num_elements].polynominal_elements.coefficient);
            printf("Enter the exponent for %d. element. (If you want to enter a constant number, set the exponent to 0.): ",i+1);
            scanf("%lf",&df_eq.f_x_function[df_eq.num_elements].polynominal_elements.exponent);
            df_eq.num_elements++;
        }
        else if(type == 2){ //trigonometrik eleman 3sin(4x^5) tarzi seyler olabilir ancak 3sin^3(4x^5) tarzi seyler girilemiyor, fakat koda biraz ekleme yaparak girdirtilebilir ancak gerek yok.
            df_eq.f_x_function = (F_X *)realloc(df_eq.f_x_function,(df_eq.num_elements+1)*sizeof(F_X));
            if(df_eq.f_x_function == NULL){
                printf("Memory allocation failed.\n");
                return 1;
            }
            df_eq.f_x_function[df_eq.num_elements].label = 1; // bu elemanin trigonometrik bir deger oldugunu belirtmek amacli bir label.
            do{
                printf("Enter the trigonometric type for %d. element: (1->sine, 2->cosine, 3-> tangent, 4-> cotangent: " ,i+1); //trigonometrik elemanin tipini girmeli.
                scanf("%d",&df_eq.f_x_function[df_eq.num_elements].trigonometric_elements.trigo_type);
            }while(df_eq.f_x_function[df_eq.num_elements].trigonometric_elements.trigo_type != 1 && df_eq.f_x_function[df_eq.num_elements].trigonometric_elements.trigo_type != 2 && df_eq.f_x_function[df_eq.num_elements].trigonometric_elements.trigo_type != 3 && df_eq.f_x_function[df_eq.num_elements].trigonometric_elements.trigo_type != 4);
            printf("Enter the trigonometric function's coefficient for %d. element (e.g. 5 for 5sin(3x^2)): ",i+1);
            scanf("%lf",&df_eq.f_x_function[df_eq.num_elements].trigonometric_elements.coefficient);
            printf("Enter the X's coefficient for %d. element (e.g. 3 for 5sin(3x^2)): ",i+1);
            scanf("%lf",&df_eq.f_x_function[df_eq.num_elements].trigonometric_elements.x.coefficient);
            printf("Enter the X's exponent for %d. element (e.g. 2 for 5sin(3x^2)): ",i+1);
            scanf("%lf",&df_eq.f_x_function[df_eq.num_elements].trigonometric_elements.x.exponent);
            
            df_eq.num_elements++;

        }
    }
    
    print_equation(df_eq);
    printf("\n");
    printf("Finally, You need to enter the initial condition values for the equation : ");
    print_equation(df_eq);
    printf("\n");
    printf("********************************************************************************************\n");
    printf("Enter the x0 value: ");
    scanf("%lf",&xi);
    printf("Enter the y0 value: ");
    scanf("%lf",&yi);
    printf("Enter the stepsize value. (h): ");
    scanf("%lf",&h);
    printf("Enter the x value for which you want to find the value of the function (the x value for the function you want to know y(x) = ? ): ");
    scanf("%lf",&x_next);
    estimated_value = runge_kutta(df_eq,xi,yi,h,x_next);
    char character;
    do{
    printf("To find the error value, you need to enter the value obtained with the actual solution of the function. Do you know the actual solution? [y/n]");
    scanf(" %c",&character);
    }while(character != 'y' && character != 'n');
    if(character == 'y'){
        printf("Enter the value obtained with the actual solution of the function.");
        scanf("%lf",&real_value);
        printf("Differantial equation => ");
        print_equation(df_eq);
        printf("\n");
        printf("The estimated value = %.20lf\n",estimated_value);
        printf("The real value = %.15lf\n",real_value);
        printf("The error = %.15lf",fabs(real_value-estimated_value));
    }
    else{
        printf("Differantial equation => ");
        print_equation(df_eq);
        printf("\nThe value at %lf : %lf",x_next,estimated_value);
    }
    free(df_eq.f_x_function);
    return 0;
}
double runge_kutta(DIFFERANTIAL_EQUATION dif_EQ, double xi, double yi, double h, double x_reach){
    static int i = 0; //bunu ekrana guzel bastirabilmek icin sabit tutmak zorundayiz ve function scope icerisinde static yapiyoruz, sadece bu fonksiyona ozel sabit deger bu scope'u asamaz..
    if (xi >= x_reach){
        printf("%d. step: x = %.4lf, y = %.15lf\n",i,xi,yi);
        return yi;
    }
    double k1 = f_xi_yi(dif_EQ,xi,yi,h);
    double k2 = f_xi_yi(dif_EQ, xi+(0.5*h), yi+(k1/2*h),h);
    double k3 = f_xi_yi(dif_EQ, xi+(0.5*h), yi+(k2/2*h),h);
    double k4 = f_xi_yi(dif_EQ,xi+h,yi+k3*h,h);
    double y_next = yi + h/6 * (k1+2*k2+2*k3+k4);
    printf("%d. step: x = %.4lf, y = %.15lf\n",i++,xi,yi);
    return runge_kutta(dif_EQ, xi + h, y_next, h, x_reach);
    
}
double f_xi_yi(DIFFERANTIAL_EQUATION dif_EQ,double xi, double yi, double h){
    // formulde f(x,y) su ifadedir : Ay' + By = f(x) diferansiyel denkleminde y' ifadesinin yalniz birakilmis hali: y' = (f(x) - By) / A;
    double f_x_y, fx = 0, By = dif_EQ.coeff_B*yi;
    int i;
    for(i = 0;i<dif_EQ.num_elements;i++){
        if (dif_EQ.f_x_function[i].label == 0){
            fx += dif_EQ.f_x_function[i].polynominal_elements.coefficient * pow(xi,dif_EQ.f_x_function[i].polynominal_elements.exponent);
        }
        if(dif_EQ.f_x_function[i].label == 1){
            if(dif_EQ.f_x_function[i].trigonometric_elements.trigo_type == 1){
                fx += dif_EQ.f_x_function[i].trigonometric_elements.coefficient * sin(dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient * pow(xi, dif_EQ.f_x_function[i].trigonometric_elements.x.exponent));
            }
            if(dif_EQ.f_x_function[i].trigonometric_elements.trigo_type == 2){
                fx += dif_EQ.f_x_function[i].trigonometric_elements.coefficient * cos(dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient * pow(xi, dif_EQ.f_x_function[i].trigonometric_elements.x.exponent));
            }
            if(dif_EQ.f_x_function[i].trigonometric_elements.trigo_type == 3){
                fx += dif_EQ.f_x_function[i].trigonometric_elements.coefficient * tan(dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient * pow(xi, dif_EQ.f_x_function[i].trigonometric_elements.x.exponent));
            }
            if(dif_EQ.f_x_function[i].trigonometric_elements.trigo_type == 4){
                fx += dif_EQ.f_x_function[i].trigonometric_elements.coefficient * (1/tan(dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient * pow(xi, dif_EQ.f_x_function[i].trigonometric_elements.x.exponent))); 
            }
        }
    }
            
    f_x_y = (fx - By) / dif_EQ.coeff_A;

    return f_x_y;
}

void print_equation(DIFFERANTIAL_EQUATION dif_EQ){
    int i;
    if(dif_EQ.coeff_A == 1){
        printf("y'",dif_EQ.coeff_A);
    }
    else{
        printf("%.2lfy'",dif_EQ.coeff_A);
    }
    
    if(dif_EQ.coeff_B>0){
        if(dif_EQ.coeff_B == 1){
            printf(" +y",dif_EQ.coeff_B);
        }
        else{
            printf(" + %.2lfy",dif_EQ.coeff_B);
        }
    }
    else if(dif_EQ.coeff_B<0){
        printf(" %.2lfy",dif_EQ.coeff_B);
    }
    printf(" = ");
    if(dif_EQ.f_x_function[0].label == 0){
        if(dif_EQ.f_x_function[0].polynominal_elements.coefficient == 1){
            printf(" x^%.2lf",dif_EQ.f_x_function[0].polynominal_elements.exponent);
        }
        else{
            printf(" %.2lfx^%.2lf",dif_EQ.f_x_function[0].polynominal_elements.coefficient,dif_EQ.f_x_function[0].polynominal_elements.exponent);
        }
        
    }
    if(dif_EQ.f_x_function[0].label == 1){
            if(dif_EQ.f_x_function[0].trigonometric_elements.trigo_type == 1){
                printf(" %.2lfsin(%.2lfx^%.2lf)",dif_EQ.f_x_function[0].trigonometric_elements.coefficient,dif_EQ.f_x_function[0].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[0].trigonometric_elements.x.exponent);
            }
            if(dif_EQ.f_x_function[0].trigonometric_elements.trigo_type == 2){
               printf(" %.2lfcos(%.2lfx^%.2lf)",dif_EQ.f_x_function[0].trigonometric_elements.coefficient,dif_EQ.f_x_function[0].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[0].trigonometric_elements.x.exponent);
            }
            if(dif_EQ.f_x_function[0].trigonometric_elements.trigo_type == 3){
                printf(" %.2lftan(%.2lfx^%.2lf)",dif_EQ.f_x_function[0].trigonometric_elements.coefficient,dif_EQ.f_x_function[0].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[0].trigonometric_elements.x.exponent);
            }
            if(dif_EQ.f_x_function[0].trigonometric_elements.trigo_type == 4){
                printf(" %.2lfcot(%.2lfx^%.2lf)",dif_EQ.f_x_function[0].trigonometric_elements.coefficient,dif_EQ.f_x_function[0].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[0].trigonometric_elements.x.exponent);
            }
    }
    for(i = 1;i<dif_EQ.num_elements;i++){

        if(dif_EQ.f_x_function[i].label == 0){
            if(dif_EQ.f_x_function[i].polynominal_elements.coefficient > 0){
                printf(" +%.2lfx^%.2lf",dif_EQ.f_x_function[i].polynominal_elements.coefficient,dif_EQ.f_x_function[i].polynominal_elements.exponent);
            }
            if(dif_EQ.f_x_function[i].polynominal_elements.coefficient < 0 ){
                printf(" %.2lfx^%.2lf",dif_EQ.f_x_function[i].polynominal_elements.coefficient,dif_EQ.f_x_function[i].polynominal_elements.exponent);
            }
                
        }
        if(dif_EQ.f_x_function[i].label == 1){
            if(dif_EQ.f_x_function[i].trigonometric_elements.trigo_type == 1){
                if(dif_EQ.f_x_function[i].trigonometric_elements.coefficient > 0){
                    printf(" +%.2lfsin(%.2lfx^%.2lf)",dif_EQ.f_x_function[i].trigonometric_elements.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.exponent);
                }
                if(dif_EQ.f_x_function[i].trigonometric_elements.coefficient < 0){
                    printf(" %.2lfsin(%.2lfx^%.2lf)",dif_EQ.f_x_function[i].trigonometric_elements.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.exponent);
                }
                
            }
            if(dif_EQ.f_x_function[i].trigonometric_elements.trigo_type == 2){
                if(dif_EQ.f_x_function[i].trigonometric_elements.coefficient > 0){
                    printf(" +%.2lfcos(%.2lfx^%.2lf)",dif_EQ.f_x_function[i].trigonometric_elements.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.exponent);
                }
                if(dif_EQ.f_x_function[i].trigonometric_elements.coefficient < 0){
                    printf(" %.2lfcos(%.2lfx^%.2lf)",dif_EQ.f_x_function[i].trigonometric_elements.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.exponent);
                }
            }
            if(dif_EQ.f_x_function[i].trigonometric_elements.trigo_type == 3){
                if(dif_EQ.f_x_function[i].trigonometric_elements.coefficient > 0){
                    printf(" +%.2lftan(%.2lfx^%.2lf)",dif_EQ.f_x_function[i].trigonometric_elements.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.exponent);
                }
                if(dif_EQ.f_x_function[i].trigonometric_elements.coefficient < 0){
                    printf(" %.2lftan(%.2lfx^%.2lf)",dif_EQ.f_x_function[i].trigonometric_elements.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.exponent);
                }
            }
            if(dif_EQ.f_x_function[i].trigonometric_elements.trigo_type == 4){
                if(dif_EQ.f_x_function[i].trigonometric_elements.coefficient > 0){
                    printf(" +%.2lfcot(%.2lfx^%.2lf)",dif_EQ.f_x_function[i].trigonometric_elements.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.exponent);
                }
                if(dif_EQ.f_x_function[i].trigonometric_elements.coefficient < 0){
                    printf(" %.2lfcot(%.2lfx^%.2lf)",dif_EQ.f_x_function[i].trigonometric_elements.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.coefficient,dif_EQ.f_x_function[i].trigonometric_elements.x.exponent);
                }
            }
        }
    }
    
}