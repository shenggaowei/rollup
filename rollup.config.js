import json from "@rollup/plugin-json";
import resolve from "@rollup/plugin-node-resolve";
import commonjs from "@rollup/plugin-commonjs";
import typescript from "@rollup/plugin-typescript";
import { terser } from "rollup-plugin-terser";

export default {
  input: "src/index.ts",
  output: {
    file: "./dist/bundle.js",
    format: "esm",
  },
  plugins: [
    commonjs({ transformMixedEsModules: true }),
    resolve(),
    json(),
    typescript({ tsconfig: "./tsconfig.json" }),
    terser({
      compress: {
        pure_getters: true,
        unsafe: true,
        unsafe_comps: true,
        warnings: false,
      },
    }),
  ],
};
