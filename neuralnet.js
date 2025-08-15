const EPSILON = 1e-15;
const sigmoid = x => 1 / (1 + Math.exp(-x));
const sigmoidDerivative = y => Math.max(EPSILON, y * (1 - y));

const createMatrix = (rows, cols) =>
  Array.from({ length: rows }, () =>
    Array.from({ length: cols }, () => (Math.random() - 0.5) * 0.1)
  );

const dotProduct = (vecA, vecB) => vecA.reduce((sum, val, i) => sum + val * vecB[i], 0);

const train = (X, Y, hiddenUnits = 2, epochs = 100000, lr = 0.1) => {
  const inputUnits = X[0].length;

  let weightsInputHidden = createMatrix(inputUnits, hiddenUnits);
  let biasHidden = Array(hiddenUnits).fill(0);

  let weightsHiddenOutput = createMatrix(hiddenUnits, 1);
  let biasOutput = 0;

  for (let epoch = 0; epoch < epochs; epoch++) {
    let totalError = 0;

    for (let i = 0; i < X.length; i++) {
      const inputVec = X[i];
      const target = Y[i][0];

      const hiddenLayer = weightsInputHidden[0].map((_, col) =>
        sigmoid(dotProduct(inputVec, weightsInputHidden.map(row => row[col])) + biasHidden[col])
      );

      const outputSum = dotProduct(hiddenLayer, weightsHiddenOutput.map(row => row[0])) + biasOutput;
      const output = sigmoid(outputSum);

      const errorOutput = target - output;
      totalError += Math.abs(errorOutput);

      const deltaOutput = errorOutput * sigmoidDerivative(output);
      const hiddenErrors = weightsHiddenOutput.map(row => row[0] * deltaOutput);
      const deltaHidden = hiddenErrors.map((err, idx) => err * sigmoidDerivative(hiddenLayer[idx]));

      for (let h = 0; h < hiddenUnits; h++) {
        weightsHiddenOutput[h][0] += lr * deltaOutput * hiddenLayer[h];
      }
      biasOutput += lr * deltaOutput;

      for (let inpIdx = 0; inpIdx < inputUnits; inpIdx++) {
        for (let h = 0; h < hiddenUnits; h++) {
          weightsInputHidden[inpIdx][h] += lr * deltaHidden[h] * inputVec[inpIdx];
        }
      }
      for (let h = 0; h < hiddenUnits; h++) {
        biasHidden[h] += lr * deltaHidden[h];
      }
    }

    if (epoch % 1000 === 0) {
      console.log(`Epoch ${epoch}: total error = ${totalError.toFixed(6)}`);
    }
  }

  return inputVec => {
    const hiddenLayer = weightsInputHidden[0].map((_, col) =>
      sigmoid(dotProduct(inputVec, weightsInputHidden.map(row => row[col])) + biasHidden[col])
    );
    const outputSum = dotProduct(hiddenLayer, weightsHiddenOutput.map(row => row[0])) + biasOutput;
    return sigmoid(outputSum);
  };
};

const inputs = [
  [0, 0],
  [0, 1],
  [1, 0],
  [1, 1],
];
const labels = [[0], [1], [1], [0]];

const model = train(inputs, labels);
inputs.forEach(input =>
  console.log(`input: ${input}, output: ${model(input).toFixed(4)}`)
);
