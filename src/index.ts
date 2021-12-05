import { version } from "./package.json";
import { get } from "lodash-es";

const obj = { name: "xix" };
function putName(name: string) {
  console.log("hha");
  console.log(version);
  const names = get(obj, "name");
  console.log("name=>", names);
  return true;
}
export default putName;
