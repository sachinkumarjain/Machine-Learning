
clft = xgb.cv(xgb_params,
	     nfold = 20,
             dtrain,
             nrounds=341,
             prediction=TRUE,
             print_every_n = 10,
             verbose=1,
             feval=deter,
	     maximize=TRUE)

score1 <- NULL
score2 <- NULL

matc <- mat.or.vec(1000,2)

for (i in 1:1000)
{

h1 <- sample(nrow(train),3409)
h2 <- setdiff(1:nrow(train), h1)


score1 <- c(score1,1-sum((clft$pred[h1]-train$y[h1])^2)/sum((train$y[h1]-mean(train$y[h1]))^2))
score2 <- c(score2,1-sum((clft$pred[h2]-train$y[h2])^2)/sum((train$y[h2]-mean(train$y[h2]))^2))

matc[i,1] <- sum((clft$pred[h1]-train$y[h1])^2)/sum((train$y[h1]-mean(train$y[h1]))^2)
matc[i,2] <- sum((clft$pred[h2]-train$y[h2])^2)/sum((train$y[h2]-mean(train$y[h2]))^2)
}

cor(1-matc[,1],1-matc[,2])
plot(1-matc[,1],1-matc[,2], ylab="public lb", xlab = "private lb")


