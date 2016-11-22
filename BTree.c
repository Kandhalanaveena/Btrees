#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct input{
int id;
char name[50];
};

struct value{
int id;
int address;
};

struct node{
struct value *key;
int n;
int index;
struct node **child;
int leaf;
struct node *parent;
};

//struct node BTree;
struct node *root;
struct value b[2000];
int ct=0;
//root=(struct node *)malloc(sizeof(struct node));

void insert_btree(struct node *x,struct value k,int p);
void Create_btree(int p);
void btree_split(struct node *x,int p);
int search(struct node *x,int a);
void traversal(struct node *x,int p);
void delete_btree(struct node *x,int a,int p);
void btree_combine(struct node *x,int p);
void btree_merge(struct node *x,struct node *y,struct node *z,int p);
void from_left(struct node *x,struct node *y,int m);
void from_right(struct node *x,struct node *y,int m);

main(int argc,char **argv){
int i,n,p,a,count=0,ch;

char nm[50];
FILE *fp;
struct input *arr;
struct value y;

fp=fopen(argv[1],"r");
if(fp==NULL) 
  printf("File cannot be opened");
fscanf(fp,"%d",&p);
n=0;
while((fscanf(fp,"%d",&a))!=EOF)
{
   n++;
   fscanf(fp,"%s",nm);
}

arr=(struct input *) malloc((n+1000)*sizeof(struct input));

fseek(fp,0L,0);
fscanf(fp,"%d",&p);
i=0;
while((fscanf(fp,"%d",&a))!=EOF)
{  
   arr[i].id=a;
   fscanf(fp,"%s",nm);
   strcpy(arr[i].name,nm);
   i++;
}

Create_btree(p);

for(count=0;count<n;count++){
   a=arr[count].id;
   ct=0;
   if(search(root,a)==1)
     continue;
   ct=0;
   y.id=a;
   y.address=count;
   insert_btree(root,y,p);
}


while(1){
printf("\n1.Search\n2.Insert\n3.delete\n4.traversal\n5.Exit\nSelect an option :\n");
scanf("%d",&ch);
switch(ch){

case 1:printf("Enter ID :");
       scanf("%d",&a);
       ct=0;
       if(search(root,a)==0)
              printf("Not found\n");
       else
       {
      
        for(i=0;i<ct;i++)
           printf("%d ",b[i].id);
           printf("%s\n",arr[b[i-1].address].name);
       }
       ct=0;
       break;
case 2:printf("Enter ID and name :");
       scanf("%d",&a);
       scanf("%s",nm);
       ct=0;
       if(search(root,a)==1)
            printf("id already exists\n");
       
       else
       {
       arr[count].id=a;
       strcpy(arr[count].name,nm);
       y.id=arr[count].id;
       y.address=count;
       insert_btree(root,y,p);
       count++;
       ct=0;
       search(root,a);
       for(i=0;i<ct;i++)
           printf("%d ",b[i].id);
           printf("%s\n",arr[b[i-1].address].name);
       ct=0;
       }
       break;
case 3:printf("Enter ID :");
       scanf("%d",&a);
       ct=0;
       if(search(root,a)==0)
              printf("No such id exists\n");
       else
       {
      delete_btree(root,a,p);
      }
       break;
case 4:
     traversal(root,p);
      break;
case 5:
       exit(0);
//default:
  //  printf
}
}
}

void Create_btree(int p){
struct node *temp;
int i;
temp=(struct node *)malloc(sizeof(struct node));
temp->leaf=1;
temp->n=0;
temp->parent=NULL;
temp->index=-1;

temp->key=(struct value *)malloc(p*sizeof(struct value));
temp->child=(struct node **)malloc((p+1)*sizeof(struct node *));
for(i=0;i<p+1;i++)
  temp->child[i]=NULL;
root=temp;
}


void insert_btree(struct node *x,struct value k,int p){
int i;
i=(x->n)-1;
if(x->leaf==1)
  {
  while(i>=0 && k.id<(x->key[i].id))
    {
     x->key[i+1]=x->key[i];
     i=i-1;
    }
    x->key[i+1]=k;
    x->n=(x->n)+1;
    if((x->n)>=p)
    btree_split(x,p);
  }
else
  {
  while(i>=0 && k.id<(x->key[i].id))
     i=i-1;
  i=i+1;
  insert_btree(x->child[i],k,p);
  }
}


void btree_split(struct node *x,int p){
struct node *z,*s,*y;
int i,l,m;
z=(struct node *)malloc(sizeof(struct node));
z->key=(struct value *)malloc(p*sizeof(struct value));
z->child=(struct node **)malloc((p+1)*sizeof(struct node *));
for(i=0;i<p+1;i++)
  z->child[i]=NULL;

z->leaf=x->leaf;

if(x->parent==NULL){
    s=(struct node *)malloc(sizeof(struct node));
    s->key=(struct value *)malloc(p*sizeof(struct value));
    s->child=(struct node **)malloc((p+1)*sizeof(struct node *));
  for(i=0;i<p+1;i++)
     s->child[i]=NULL;
   root=s;
   s->index=-1;
   s->parent=NULL;
   s->n=0;
   s->child[0]=x;
   x->parent=s;
   x->index=0; 
}
y=x->parent;

if(p%2 !=0){
l=(p-1)/2;
z->n=l;
for(i=0;i<l;i++)
  z->key[i]=x->key[l+i+1];  
if(x->leaf==0)
  {
   for(i=0;i<=l;i++)
    {
     z->child[i]=x->child[l+i+1];
     z->child[i]->parent=z;
     z->child[i]->index=i;
     }
  }
}

else{
l=(p+1)/2;
z->n=p-l-1;
for(i=0;i<p-l-1;i++)
   z->key[i]=x->key[l+i+1];
if(x->leaf==0)
  {
   for(i=0;i<=p-l-1;i++)
    {
     z->child[i]=x->child[l+i+1];
     z->child[i]->parent=z;
     z->child[i]->index=i;
    }
  }
}

x->n=l;
m=x->index;
for(i=(y->n);i>=m+1;i--)
 {
  y->child[i+1]=y->child[i];
  y->child[i+1]->index=i+1;
 }
y->child[m+1]=z;
z->index=m+1;
z->parent=y;

for(i=(y->n)-1;i>=m;i--)
  y->key[i+1]=y->key[i];
y->key[m]=x->key[l];

y->n=(y->n)+1;
if((y->n)>=p)
   btree_split(y,p);
}


int search(struct node *x,int a){
int i=0,m;
if(x==NULL)
   return;
else if(x->leaf ==1)
{
   while(i<(x->n) && a>((x->key[i]).id))
   {
      b[ct]=x->key[i];
      ct=ct+1;
      i=i+1;
     } 
  if(i<(x->n) && a==((x->key[i]).id))
    {
      b[ct]=x->key[i];
      ct=ct+1;
    return 1;
    }
  else
    return 0;
 }
else
{
while(i<(x->n) && a>((x->key[i]).id))
   {
      b[ct]=x->key[i];
      ct=ct+1;
      i=i+1;
     } 
if(i<(x->n) && a==((x->key[i]).id))
  {
   b[ct]=x->key[i];
      ct=ct+1;
    return 1;
   }
else 
    m=search(x->child[i],a);
}

}

void traversal(struct node *x,int p){
int i,j;
if(x==NULL)
   return;
else if(x->leaf ==1)
{
  for(i=0;i<(x->n);i++)
    printf("%d ",(x->key[i]).id);
}
else
{
for(i=0;i<(x->n);i++)
  {
  
   traversal(x->child[i],p);
   printf("%d ",(x->key[i]).id);
  }
  traversal(x->child[i],p);
}
}


void delete_btree(struct node *x,int a,int p){
int i=0,j,l;
l=((p+1)/2);
struct node *s,*t;
struct value f,k;
while(i<(x->n) && a>((x->key[i]).id))
     i=i+1;

if(i<(x->n) && a==((x->key[i]).id))    
 {
  k=x->key[i];
  if(x->leaf == 1)              
     {
      
        for(j=i+1;j<(x->n);j++)
          x->key[j-1]=x->key[j];
        x->n=(x->n)-1;
        
      if((x->n)< l-1 && (x->parent)!= NULL)
         btree_combine(x,p);
     }
   else
     {
       //s=find_predecessor(x->child[i],a);
       //t=find_successor(x->child[i+1],a);
      if(x->child[i]!=NULL)
       {
         s=x->child[i];
         while(s!=NULL && s->leaf == 0)
            s=s->child[s->n];
        }
      if(x->child[i+1]!=NULL)
       {
        t=x->child[i+1];
        while(t!=NULL && t->leaf == 0)
           t=t->child[0];
       }
       if(s->n >=l)
           {f=s->key[(s->n)-1];
           x->key[i]=f;
           delete_btree(s,f.id,p);
          }
       else if(t->n >=l)
          {
            f=t->key[0];
           x->key[i]=f;
           delete_btree(t,f.id,p);
          }
       else
        { 
          f=s->key[(s->n)-1];
           x->key[i]=f;
           delete_btree(s,f.id,p);
         }
      }
  }
else
  {
    if(i==x->n && a>((x->key[i-1]).id))
    {
      
      if((x->child[i])!=NULL)
       {
       delete_btree(x->child[i],a,p);
       }
    }
    else if(i<(x->n) && a<(x->key[i].id))
    {
      
      if((x->child[i])!=NULL)
       {
       delete_btree(x->child[i],a,p);
       }
    }
 }
}


void btree_combine(struct node *x,int p){
int l,m;
struct node *y,*z;
l=((p+1)/2);
m=x->index;
y=x->parent;
if(m==0){
  z=y->child[1];

if((z->n) >=l)
     from_right(x,y,m);
    else
     {
      z=y->child[m+1];
      btree_merge(x,y,z,p);
     }
}


else if(m==(y->n)){
  z=y->child[m-1];
if((z->n) >=l)
     from_left(x,y,m);
    else
    {
     z=y->child[m-1];
     btree_merge(z,y,x,p);
    }
}

else{
  z=y->child[m+1];
  if((z->n) >=l)
     from_right(x,y,m);
 else{ 
   z=y->child[m-1];
   if((z->n) >=l)
     from_left(x,y,m);
   else
     {
      z=y->child[m+1];
      btree_merge(x,y,z,p);
     }
  }
}
}


void btree_merge(struct node *x,struct node *y,struct node *z,int p){
int i,l,m;
l=((p+1)/2);
m=x->index;
for(i=0;i<(z->n);i++)
   x->key[(x->n)+i+1]=z->key[i];
x->key[(x->n)]=y->key[m];

if(z->leaf == 0)
 {
  for(i=0;i<=(z->n);i++)
   {
     x->child[(x->n)+i+1]=z->child[i];
     x->child[(x->n)+i+1]->parent=x;
     x->child[(x->n)+i+1]->index=(x->n)+i+1;
   }
 }
 x->n = (x->n)+(z->n) +1;


for(i=m+1;i<(y->n);i++)
  y->key[i-1]=y->key[i];
  
if(y->leaf ==0)
 {
  for(i=m+2;i<=(y->n);i++)
   {
    y->child[i-1]=y->child[i];
    y->child[i-1]->index=i-1;
   }
 }
 y->n = (y->n) - 1;
 if((y->n)==0 && (y->parent)==NULL)
     {root=x;
      x->parent=NULL;
      x->index=-1;
     // free(y);
      }

 free(z);
if((y->n)< l-1 && (y->parent)!= NULL)
    btree_combine(y,p);
}






void from_right(struct node *x,struct node *y,int m){
struct node *z;
int i;
z=y->child[m+1];
   x->key[x->n]=y->key[m];
      x->n=x->n +1;
      y->key[m]=z->key[0];
      i=1;
      while(i<(z->n))
         {z->key[i-1]=z->key[i];
          i=i+1;
         }
      if(z->leaf == 0)
        {
         x->child[x->n]=z->child[0];
         x->child[x->n]->parent=x;
         x->child[x->n]->index=x->n;
        i=1;
        while(i<=(z->n))
         {z->child[i-1]=z->child[i];
          z->child[i-1]->index=i-1;
          i=i+1;
         }
        }  
         z->n =(z->n)-1;
}  

void from_left(struct node *x,struct node *y,int m){
struct node *z;
int i;
z=y->child[m-1];
      i=(x->n)-1;
      while(i>=0)
         {x->key[i+1]=x->key[i];
          i=i-1;
         }
         x->key[0]=y->key[m-1];
       if(z->leaf == 0)
       {
        i=x->n;
        while(i>=0)
         {x->child[i+1]=x->child[i];
          x->child[i+1]->index=i+1;
          i=i-1;
         }
         x->child[0]=z->child[z->n];
         x->child[0]->index=0;
         x->child[0]->parent=x;
        }
         x->n =(x->n)+1;
         y->key[m-1]=z->key[(z->n)-1];
         z->n=(z->n) -1;
}  

     
